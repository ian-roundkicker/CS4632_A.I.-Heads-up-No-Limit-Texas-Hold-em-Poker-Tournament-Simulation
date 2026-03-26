#include "WeightedRandom.h"

WeightedRandom::WeightedRandom() : mt(std::random_device{}()) {
	// nested for loop to modify contents of weights array
	// for each member of weights, convert it into the a floating point number by dividing it by 100, apply the 4th root of that number, and then multiply it by 100 again to get the new weight value
	// this is done because this weight will be used as a probability to decide whether to fold, call, or raise 4 times. This will change 85% chance to fold each turn into 85% chance to not fold on any of the four turns.
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			float probability = weights[i][j] / 100.0f;
			probability = std::pow(probability, 0.25f);
			weights[i][j] = static_cast<int>(std::round(probability * 100));
		}
	}
}

int WeightedRandom::decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) {
	int choice = std::uniform_int_distribution<int>(0, 100)(mt);
	int chance = weights[13-(own_cards->at(0)->card_num / 4)][13-(own_cards->at(1)->card_num / 4)];
	if (choice > chance && current_bet < current_chips) {
		return -1; // fold
	}
	else if (choice < chance / 4) {// ensures 25% chance to raise if they don't fold
		if (current_chips > current_bet) {
			return std::uniform_int_distribution<int>(1, current_chips - current_bet)(mt); // raise by a random amount between 1 and the maximum they can raise
		}
		else {
			return 0; // if they don't have enough chips to raise, they will call instead
		}
	}
	else {
		return 0; // call
	}
}