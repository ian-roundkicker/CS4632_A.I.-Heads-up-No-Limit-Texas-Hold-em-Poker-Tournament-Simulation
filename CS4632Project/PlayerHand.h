#pragma once
// pointing to this directory's Card.h, not PHEvaluator's Card.h
#include "Card.h"
#include "A_RandomAction.h"
// this is here so that I can use std::shuffle
#include <algorithm>
#include <phevaluator/phevaluator.h>

// class PlayerHand represents one player.
class PlayerHand {
public:
    // uses a vector of pointers to Cards in deck[]
    // Game class is responsible for handling assignment of cards
    std::vector<Card*> cards;
    // todo: consider using a static variable to handle player_id if c++ has the feature
    int player_id;
    int available_chips;
	A_RandomAction behavior;

    // constructor sets up the player_id and card vector
    PlayerHand(int player_id);

    // param c is a pointer to a card (Game class is responsible for managing this)
    void give_card(Card* c);

    // clears the card pointer array.
    void clear_cards();

    // sorts the cards for reading simplicity when print_hand is called
    void sort();

    // print_hand lists out all the cards in the player's deck
    void print_hand();

    // overloaded method returns the rank  of the hand when community cards are considered.
    int getRank(std::array<Card*, 5> community_cards);

};

