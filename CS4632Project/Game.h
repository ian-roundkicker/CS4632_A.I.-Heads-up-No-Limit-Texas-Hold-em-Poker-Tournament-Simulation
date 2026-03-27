#pragma once
#include "PlayerHand.h"
// std::random_device and std::mt19937
#include <random>
// sqlite-amalgamation version 3.51.3
#include <sqlite3.h>
#include <string.h>
class Game {// Game class (formerly Deck class) handles core simulation logic
public:
    // array of type Card because I wanted the array to be of a fixed size. 
    Card deck[52];
    // represents the players
    std::vector<PlayerHand> players;
    // represents players still in the game
    std::vector<PlayerHand*> active_players;
    // represents players in the current bet
    std::vector<PlayerHand*> betting_players;
    // counts the number of cards handed out so that no two players get the same card from deck[]
    int cards_handed;
    std::array<Card*, 5> community_cards;
    int current_bet;
    // rd and mt are required for shuffling
    std::random_device rd = std::random_device();
    std::mt19937 mt = std::mt19937(rd());

    //initializes the cards and the players. Does not hand out any cards
    Game(int player1, int player2);

	void handle_bets();

    // made this function out of desperation
    void temp_postbethandle(sqlite3* db, int round_number, int game_id);

    // Simulates one poker game. We are on texas hold'em
    int playGame(int num_chips, sqlite3* db, int game_id);

    // Prints out hands in the order of least to greatest (which is also best hand to worst hand)
    void rank_hands(sqlite3* db, int round_number, int game_id);

    // clears the cards from each player's hand and resets cards_handed
    void clear_cards();

    // shuffles the cards
    void Shuffle();
};

