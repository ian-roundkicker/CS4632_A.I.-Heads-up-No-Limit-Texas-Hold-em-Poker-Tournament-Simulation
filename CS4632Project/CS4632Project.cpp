// this is here because it is here by default
#include <iostream>
// this is here so that I can use std::shuffle
#include <algorithm>
// std::random_device and std::mt19937
#include <random>
// for std::begin() and std::end()
#include <iterator>
//phevaluator library
#include <phevaluator/phevaluator.h>
#include <phevaluator/card.h>
#include <phevaluator/card_sampler.h>
#include <phevaluator/rank.h>

// represents a single card
class Card {
public:
    int card_num;

    // default constructor exists so that I can use an Array for the card
    Card() {
        card_num = -1;
    }

    // overloaded constructor is what is primarily used
    Card(int num) {
        card_num = num;
    }

    // returns a string representing the name and suit of the card
    std::string getName() {
        std::string name = "";
        std::string suit = "";
        int card_category = card_num / 4;
        switch (card_category) {
        case 0:
            name = "2";
            break;
        case 1:
            name = "3";
            break;
        case 2:
            name = "4";
            break;
        case 3:
            name = "5";
            break;
        case 4:
            name = "6";
            break;
        case 5:
            name = "7";
            break;
        case 6:
            name = "8";
            break;
        case 7:
            name = "9";
            break;
        case 8:
            name = "10";
            break;
        case 9:
            name = "Judge";
            break;
        case 10:
            name = "Queen";
            break;
        case 11:
            name = "King";
            break;
        case 12:
            name = "Ace";
            break;
        }
        card_category = card_num % 4;
        switch (card_category) {
        case 0:
            suit = "Clubs";
            break;
        case 1:
            suit = "Diamonds";
            break;
        case 2:
            suit = "Hearts";
            break;
        case 3:
            suit = "Spades";
            break;
        }
        return name + " of " + suit;
    }
};
// class PlayerHand represents one player. Later I intend to give a number of chips to the player
class PlayerHand {
public:
    // uses a vector of pointers to Cards in deck[]
    // Game class is responsible for handling assignment of cards
    std::vector<Card*> cards;
    // todo: consider using a static variable to handle player_id if c++ has the feature
    int player_id;
    //
    int available_chips;

    // constructor sets up the player_id and card vector
    PlayerHand(int player_id) {
        cards = std::vector<Card*>();
        this->player_id = player_id;
        available_chips = 5;
    }

    // param c is a pointer to a card (Game class is responsible for managing this)
    void give_card(Card* c) {
        if (cards.size() < 5) {
            cards.push_back(c);
        }
        else {
            throw std::exception("Invalid number of cards to get rank.");
        }
    }

    // clears the card pointer array.
    void clear_cards() {
        cards.clear();
    }

    // sorts the cards for reading simplicity when print_hand is called
    void sort() {
        std::sort(std::begin(cards), std::end(cards), [](const Card* a, const Card* b) {return a->card_num < b->card_num;});
    }

    // print_hand lists out all the cards in the player's deck
    void print_hand() {
        for (Card* c : cards) {
            if (c == nullptr) {
                return;
            }
            std::cout << c->getName() << "\n";
        }
    }

    // getRank gets the rank of the current poker hand, I will have to use an overloaded method when community cards are to be considered
    // also lower ranks mean a better hand
    int getRank() {
        switch (cards.size()) {
        case 5:
            return evaluate_5cards(cards.at(0)->card_num, cards.at(1)->card_num, cards.at(2)->card_num, cards.at(3)->card_num, cards.at(4)->card_num);
        default:
            throw std::exception("Invalid number of cards to get rank.");
        }
    }

    int getRank(std::array<Card*, 5> community_cards) {
		return evaluate_7cards(cards.at(0)->card_num, cards.at(1)->card_num, community_cards.at(0)->card_num, community_cards.at(1)->card_num, community_cards.at(2)->card_num, community_cards.at(3)->card_num, community_cards.at(4)->card_num);
    }


};

// Game class (formerly Deck class) handles core simulation logic
class Game {
public:
    // array of type Card because I wanted the array to be of a fixed size. 
    Card deck[52];
    // represents the players
    std::vector<PlayerHand> players;
    // represents players still in the game
	std::vector<PlayerHand*> active_players;
    // counts the number of cards handed out so that no two players get the same card from deck[]
    int cards_handed;
	std::array<Card*, 5> community_cards;
    int current_bet;
    // rd and mt are required for shuffling
    std::random_device rd = std::random_device();
    std::mt19937 mt = std::mt19937(rd());

    //initializes the cards and the players. Does not hand out any cards
    Game() {
        cards_handed = 0;
        // initialize cards and add to deck
        for (int i = 0; i < 52; i++) {
            deck[i] = Card(i);
        }
        // initialize players
        players = std::vector<PlayerHand>();
        for (int i = 0; i < 2; i++) {
            players.push_back(PlayerHand(i+1));
        }
		
		// initilize active_players to contain all the players at the start of the game
        active_players = std::vector<PlayerHand*>();
		community_cards = std::array<Card*, 5>();
        
    }

    // Simulates one poker game. As of this commit, we are currently at basic 5-card poker
    void playGame() {
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

            // hand pre-flop betting

            // The flop
            for (int i = 0; i < 3; i++) {
                community_cards.at(i) = &deck[cards_handed];
				cards_handed++;
            }

            // handle raising and player behavior here.

			// The turn
            community_cards.at(3) = &deck[cards_handed];
			cards_handed++;

			// handle raising and player behavior here.

			// The river
            community_cards.at(4) = &deck[cards_handed];
			cards_handed++;

			// handle raising and player behavior here.

            for (int i = 0; i < 5; i++) {
                std::cout << "Community card " << i + 1 << ": " << community_cards.at(i)->getName() << "\n";
            }

            rank_hands();
        }
        
    }

    // Prints out hands in the order of least to greatest (which is also best hand to worst hand)
    void rank_hands() {
        std::vector<PlayerHand*> active_players_copy = active_players;
        std::sort(active_players_copy.begin(), active_players_copy.end(), [this](PlayerHand* a, PlayerHand* b) {return a->getRank(this->community_cards) < b->getRank(this->community_cards);});
        for (int i = 1; i <= active_players_copy.size(); i++) {
            std::cout << "Player " << active_players_copy.at(i - 1)->player_id << " was at rank " << i << "\n";
        }
        for (int i = 0; i < active_players.size(); i++) {
            if (active_players.at(i)->player_id != active_players_copy.at(0)->player_id) {
                // give the chips to the winner and take the chips from the loser
				active_players_copy.at(0)->available_chips += current_bet;
                active_players.at(i)->available_chips -= current_bet;
                if (active_players.at(i)->available_chips <= 0) {
                    active_players.erase(active_players.begin() + i);
                    i--;
                }
            }
        }
		std::cout << "Player " << active_players_copy.at(0)->player_id << " won the hand and now has " << active_players_copy.at(0)->available_chips << " chips.\n";

    }

    // clears the cards from each player's hand and resets cards_handed
    void clear_cards() {
        cards_handed = 0;
        for (int i = 0; i < players.size(); i++) {
            players.at(i).clear_cards();
        }
    }

    // shuffles the cards
    void Shuffle() {
        std::shuffle(std::begin(deck), std::end(deck), mt);
    }
};

//main method
int main()
{
    // initialize the game
    Game d1 = Game();
    // demonstrate deck
    std::cout << "Sorted deck:" << "\n";
    for (Card c : d1.deck) {
        std::cout << c.getName() << "\n";
    }
    // demonstrate sorting function
    std::cout << "\n\nUnsorted deck:" << "\n";
    d1.Shuffle();
    for (Card c : d1.deck) {
        std::cout << c.getName() << "\n";
    }
    for (int i = 1; i <= 1; i++) {
        std::cout << "\n\n\nGAME " << i << "\n";
		d1.playGame();
    }
    
    std::cout << "END OF PROGRAM";
}

// default VS template comments here because I might make use of the information later.

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
