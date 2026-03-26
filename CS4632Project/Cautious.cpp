#include "Cautious.h"

Cautious::Cautious() {
}

int Cautious::decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) {
	//check community cards to see which round we are on.
	int hand_strength;
	int round;
	if (community_cards->at(0) == nullptr) {
		//pre-flop
		hand_strength = 0; // unknown hand strength
		round = 0;
	}
	else if (community_cards->at(3) == nullptr) {
		//flop
		hand_strength = evaluate_5cards(own_cards->at(0)->card_num, own_cards->at(1)->card_num, community_cards->at(0)->card_num, community_cards->at(1)->card_num, community_cards->at(2)->card_num);
		round = 1;
	}
	else if (community_cards->at(4) == nullptr) {
		//turn
		hand_strength = evaluate_6cards(own_cards->at(0)->card_num, own_cards->at(1)->card_num, community_cards->at(0)->card_num, community_cards->at(1)->card_num, community_cards->at(2)->card_num, community_cards->at(3)->card_num);
		round = 2;
	}
	else {
		// river
		hand_strength = evaluate_7cards(own_cards->at(0)->card_num, own_cards->at(1)->card_num, community_cards->at(0)->card_num, community_cards->at(1)->card_num, community_cards->at(2)->card_num, community_cards->at(3)->card_num, community_cards->at(4)->card_num);
		round = 3;
	}
	if (hand_strength < 7462 / (round+2)) {
		// call if can't add more chips to pot or flop hasn't happened yet, raise if can add more chips to pot
		return current_bet >= current_chips || round == 0 ? 0 : 1;
	}
	return current_bet >= current_chips ? 0 : -1;

}