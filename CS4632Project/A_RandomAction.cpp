#include "A_RandomAction.h"

A_RandomAction::A_RandomAction() : mt(std::random_device{}()) {
	// constructor can be empty since we don't have any member variables to initialize
}

int A_RandomAction::decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) {
	int choice = std::uniform_int_distribution<int>(-1, 1)(mt); // -1 for fold, 0 for call, 1 or greater to raise by that amount
	if (choice == -1) {
		return -1; // fold
	}
	else if (choice == 0) {
		return 0; // call
	}
	else {
		if (current_chips > current_bet) {
			return std::uniform_int_distribution<int>(1, current_chips - current_bet)(mt); // raise by a random amount between 1 and the maximum they can raise
		}
		else {
			return 0; // if they don't have enough chips to raise, they will call instead
		}
	}

}

