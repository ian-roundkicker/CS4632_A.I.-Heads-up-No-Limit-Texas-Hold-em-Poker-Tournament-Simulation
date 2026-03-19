#include "Game.h"

Game::Game() {
	current_bet = 0;
    cards_handed = 0;
    // initialize cards and add to deck
    for (int i = 0; i < 52; i++) {
        deck[i] = Card(i);
    }
    // initialize players
    players = std::vector<PlayerHand>();
    for (int i = 0; i < 2; i++) {
        players.push_back(PlayerHand(i + 1));
    }

    // initilize active_players to contain all the players at the start of the game
    active_players = std::vector<PlayerHand*>();
    community_cards = std::array<Card*, 5>();

}

//todo: change active_players to betting_players
void Game::handle_bets() {
    //reset the loop when someone raises.
	int proposed_bet = current_bet;
    int decision;
    for (int i = 0; i < betting_players.size(); i++) {
		decision = betting_players.at(i)->behavior.decideAction(&betting_players.at(i)->cards, &community_cards, proposed_bet, betting_players.at(i)->available_chips);
        if (decision == -1) {
            std::cout << "Player " << betting_players.at(i)->player_id << " folded.\n";
            betting_players.erase(betting_players.begin() + i);
            // change the line below if there are more than 2 players
            return;
        }
        else if (decision == 0) {
            std::cout << "Player " << betting_players.at(i)->player_id << " called.\n";
        }
        else {
            // set the current bet to the previously preposed bet
			current_bet = proposed_bet;
            proposed_bet += decision;
            std::cout << "Player " << betting_players.at(i)->player_id << " raised to " << proposed_bet << ".\n";
            if (i == 0) {
                continue;
            }
            else {
                i = -1;
            }
        }
    }
	current_bet = proposed_bet;
}

void Game::temp_postbethandle() {
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
}

void Game::playGame() {
    if (active_players.size() > 0) {
        active_players.clear();
    }
    for (int i = 0; i < players.size(); i++) {
        active_players.push_back(&players.at(i));
    }
    for (PlayerHand* p : active_players) {
        p->available_chips = 5;
    }
    for (int i = 0; i < 5; i++) {
        community_cards.at(i) = nullptr;
    }
    while (active_players.size() > 1) {
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

        // hand pre-flop betting
		handle_bets();
        if (betting_players.size() == 1) {
            temp_postbethandle();
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
            temp_postbethandle();
            continue;
        }

        // The turn
        community_cards.at(3) = &deck[cards_handed];
        cards_handed++;
		std::cout << "The fourth community card is: " << community_cards.at(3)->getName() << "\n\n";

        // handle raising and player behavior here.
        handle_bets();
        if (betting_players.size() == 1) {
            temp_postbethandle();
            continue;
        }

        // The river
        community_cards.at(4) = &deck[cards_handed];
        cards_handed++;
		std::cout << "The fifth community card is: " << community_cards.at(4)->getName() << "\n\n";

        // handle raising and player behavior here.
        handle_bets();
        if (betting_players.size() == 1) {
            temp_postbethandle();
            continue;
        }

        std::cout << "\n";
        /*for (int i = 0; i < 5; i++) {
            std::cout << "Community card " << i + 1 << ": " << community_cards.at(i)->getName() << "\n";
        }*/

        rank_hands();
    }

}

void Game::rank_hands() {
	//copy active_players to a new vector so that we can sort the new vector without changing the order of active_players
    std::vector<PlayerHand*> active_players_copy = active_players;
    std::sort(active_players_copy.begin(), active_players_copy.end(), [this](PlayerHand* a, PlayerHand* b) {return a->getRank(this->community_cards) < b->getRank(this->community_cards);});
    for (int i = 1; i <= active_players_copy.size(); i++) {
        std::cout << "Player " << active_players_copy.at(i - 1)->player_id << " was at rank " << i << "\n";
    }
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