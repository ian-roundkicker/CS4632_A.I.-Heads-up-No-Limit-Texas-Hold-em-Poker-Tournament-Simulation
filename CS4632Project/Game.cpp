#include "Game.h"


static void update_winner(sqlite3* db, int game_id, int round_number, std::array<Card*, 5> community_cards, PlayerHand* winner) {
    const char* sql = "UPDATE round SET RoundWinner = ?, RevealedCC = ? WHERE GameID = ? AND RoundNumber = ?;";
    sqlite3_stmt* stmt;
    std::string revealed_cc_str;
    const char* revealed_cc;
    if (community_cards.at(0) == nullptr) {
        revealed_cc_str = "{}";
    }
    else if (community_cards.at(3) == nullptr) {
        revealed_cc_str = "{" + std::to_string(community_cards.at(0)->card_num) + "," + std::to_string(community_cards.at(1)->card_num) + "," + std::to_string(community_cards.at(2)->card_num) + "}";
    }
    else if (community_cards.at(4) == nullptr) {
        revealed_cc_str = "{" + std::to_string(community_cards.at(0)->card_num) + "," + std::to_string(community_cards.at(1)->card_num) + "," + std::to_string(community_cards.at(2)->card_num) + "," + std::to_string(community_cards.at(3)->card_num) + "}";
    }
    else {
        revealed_cc_str = "{" + std::to_string(community_cards.at(0)->card_num) + "," + std::to_string(community_cards.at(1)->card_num) + "," + std::to_string(community_cards.at(2)->card_num) + "," + std::to_string(community_cards.at(3)->card_num) + "," + std::to_string(community_cards.at(4)->card_num) + "}";
    }
	revealed_cc = revealed_cc_str.c_str();
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_bind_int(stmt, 1, winner->player_id) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 2, revealed_cc, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_int(stmt, 3, game_id) != SQLITE_OK || 
            sqlite3_bind_int(stmt, 4, round_number) != SQLITE_OK) {
        std::cout << "Error binding parameters: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "Error executing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    sqlite3_finalize(stmt);
}

static void insert_round(sqlite3* db, PlayerHand* p1, PlayerHand* p2, int round_number, int game_id) {
    const char* sql = "INSERT INTO round (GameID, RoundNumber, B1Hand, B2Hand) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    std::string temp_string = "{" + std::to_string(p1->cards.at(0)->card_num) + "," + std::to_string(p1->cards.at(1)->card_num) + "}";
    const char* p1_hand_str = temp_string.c_str();
    temp_string = "{" + std::to_string(p2->cards.at(0)->card_num) + "," + std::to_string(p2->cards.at(1)->card_num) + "}";
    const char* p2_hand_str = temp_string.c_str();
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_bind_int(stmt, 1, game_id) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, round_number) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, p1_hand_str, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 4, p2_hand_str, -1, SQLITE_TRANSIENT) != SQLITE_OK)
        {
        std::cout << "Error binding parameters: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "Error executing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    sqlite3_finalize(stmt);
}

Game::Game(int player1, int player2) {
	current_bet = 0;
    cards_handed = 0;
    // initialize cards and add to deck
    for (int i = 0; i < 52; i++) {
        deck[i] = Card(i);
    }
    // initialize players
    players = std::vector<PlayerHand>();
    players.push_back(PlayerHand(1, player1));
	players.push_back(PlayerHand(2, player2));

    // initilize active_players to contain all the players at the start of the game
    active_players = std::vector<PlayerHand*>();
    community_cards = std::array<Card*, 5>();

}

void Game::handle_bets() {
    int proposed_bet = current_bet;
    int decision;
    for (int i = 0; true; i++) {
        decision = betting_players.at(i % 2)->behavior->decideAction(&betting_players.at(i % 2)->cards, &community_cards, proposed_bet, betting_players.at(i % 2)->available_chips);
        if (decision == -1) {
            std::cout << "Player " << betting_players.at(i%2)->player_id << " folded.\n";
            betting_players.erase(betting_players.begin() + i%2);
            // change the line below if there are more than 2 players
            return;
        }
        else if (decision == 0) {
            std::cout << "Player " << betting_players.at(i%2)->player_id << " called.\n";
            if (i > 0) {
                current_bet = proposed_bet;
                return;
            }
        }
        else {
            // set the current bet to the previously preposed bet
            current_bet = proposed_bet;
            proposed_bet += decision;
            std::cout << "Player " << betting_players.at(i%2)->player_id << " raised to " << proposed_bet << ".\n";
        }
    }
}

void Game::temp_postbethandle(sqlite3* db, int round_number, int game_id) {
    int other_player_winnings = 0;
    for (int i = 0; i < active_players.size(); i++) {
        if (active_players.at(i)->player_id != betting_players.at(0)->player_id) {
            other_player_winnings = std::min(current_bet, active_players.at(i)->available_chips);
            active_players.at(i)->available_chips -= other_player_winnings;
            //add line giving chips to winner if heads up gets removed
            if (active_players.at(i)->available_chips <= 0) {
                active_players.erase(active_players.begin() + i);
                i--;
            }
        }
    }
    betting_players.at(0)->available_chips += other_player_winnings;
    std::cout << "Player " << betting_players.at(0)->player_id << " won the hand and now has " << betting_players.at(0)->available_chips << " chips.\n";
	update_winner(db, game_id, round_number, community_cards, betting_players.at(0));
}

int Game::playGame(int num_chips, sqlite3* db, int game_id) { // it's probably bad practice to have the database be required to be open by default but I'm just using this once so why bother?
    //reset active players to contain all the players at the start of the game
    if (active_players.size() > 0) {
        active_players.clear();
    }
    for (int i = 0; i < players.size(); i++) {
        active_players.push_back(&players.at(i));
    }
    for (PlayerHand* p : active_players) {
        p->available_chips = num_chips; // set available chips to the value in the JSON
    }
	//reset community cards pointers
    for (int i = 0; i < 5; i++) {
        community_cards.at(i) = nullptr;
    }
    //change to for loop to increment round number
    for (int round_number = 1; active_players.size() > 1; round_number++) {
        // If we have already run this function and I forgot to clear the cards, clear the cards from each player's deck
        if (cards_handed > 0) {
            clear_cards();
        }
        // Shuffle cards before dealing
        Shuffle();

        // deal cards
        for (int i = 0; i < 2 * active_players.size(); i++) {
            active_players.at(i % active_players.size())->give_card(&deck[cards_handed]); // note: cards_handed used to be an i.
            cards_handed++;
        }


        // todo: initialize bet here
        current_bet = 1;

        // sort each player's deck because I intend to print the hands next.
        for (int i = 0; i < active_players.size(); i++) {
            active_players.at(i)->sort();
        }

        // Print initial hands
        for (int i = 1; i <= active_players.size(); i++) {
            std::cout << "\nPlayer " << active_players.at(i - 1)->player_id << "\'s hand:\n";
            active_players.at(i - 1)->print_hand();
        }
		std::cout << "\n";

        betting_players = active_players;

		insert_round(db, active_players.at(0), active_players.at(1), round_number, game_id);

        // hand pre-flop betting
		handle_bets();
        if (betting_players.size() == 1) {
			temp_postbethandle(db, round_number, game_id);
            continue;
		}
        // subtract the chips from players in 


        // The flop
        for (int i = 0; i < 3; i++) {
            community_cards.at(i) = &deck[cards_handed];
            cards_handed++;
        }
		std::cout << "The first three community cards are: \n" << community_cards.at(0)->getName() << "\n" << community_cards.at(1)->getName() << "\n" << community_cards.at(2)->getName() << "\n\n";
        

        // handle raising and player behavior here.
        handle_bets();
        if (betting_players.size() == 1) {
            temp_postbethandle(db, round_number, game_id);
            continue;
        }

        // The turn
        community_cards.at(3) = &deck[cards_handed];
        cards_handed++;
		std::cout << "The fourth community card is: " << community_cards.at(3)->getName() << "\n\n";

        // handle raising and player behavior here.
        handle_bets();
        if (betting_players.size() == 1) {
            temp_postbethandle(db, round_number, game_id);
            continue;
        }

        // The river
        community_cards.at(4) = &deck[cards_handed];
        cards_handed++;
		std::cout << "The fifth community card is: " << community_cards.at(4)->getName() << "\n\n";

        // handle raising and player behavior here.
        handle_bets();
        if (betting_players.size() == 1) {
            temp_postbethandle(db, round_number, game_id);
            continue;
        }

        std::cout << "\n";

        rank_hands(db, round_number, game_id);
		
    }
	return active_players.at(0)->player_id;
}

void Game::rank_hands(sqlite3* db, int round_number, int game_id) {
	//copy active_players to a new vector so that we can sort the new vector without changing the order of active_players
    std::vector<PlayerHand*> active_players_copy = active_players;
    std::sort(active_players_copy.begin(), active_players_copy.end(), [this](PlayerHand* a, PlayerHand* b) {return a->getRank(this->community_cards) < b->getRank(this->community_cards);});
    for (int i = 1; i <= active_players_copy.size(); i++) {
        std::cout << "Player " << active_players_copy.at(i - 1)->player_id << " was at rank " << i << "\n";
    }
	update_winner(db, game_id, round_number, community_cards, active_players_copy.at(0));
	int winnings = 0;
    for (int i = 0; i < active_players.size(); i++) {
        if (active_players.at(i)->player_id != active_players_copy.at(0)->player_id) {
            // give the chips to the winner and take the chips from the loser
			winnings = std::min(current_bet, active_players.at(i)->available_chips);
            active_players_copy.at(0)->available_chips += winnings;
            active_players.at(i)->available_chips -= winnings;
            if (active_players.at(i)->available_chips <= 0) {
                active_players.erase(active_players.begin() + i);
                i--;
            }
        }
    }
    std::cout << "Player " << active_players_copy.at(0)->player_id << " won the hand and now has " << active_players_copy.at(0)->available_chips << " chips.\n";

}

void Game::clear_cards() {
    cards_handed = 0;
    for (int i = 0; i < players.size(); i++) {
        players.at(i).clear_cards();
    }
}

void Game::Shuffle() {
    std::shuffle(std::begin(deck), std::end(deck), mt);
}