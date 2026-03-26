#include "RaiseBot.h"

RaiseBot::RaiseBot() {
}

int RaiseBot::decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) {
	// This bot will always raise by 1 if it can. If it can't raise, it'll bet its mother's soul. Since this is a poker simulation and not Jojo's Bizarre Adventure, I'll just have to call.
	if (current_bet >= current_chips) {
		return 0;
	}
	else {
		return 1;
	}
}
