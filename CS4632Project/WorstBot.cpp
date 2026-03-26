#include "WorstBot.h"

WorstBot::WorstBot() {
	// constructor doesn't need to do anything
}

int WorstBot::decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) {
	// This bot always folds.
	return -1; // fold
}
