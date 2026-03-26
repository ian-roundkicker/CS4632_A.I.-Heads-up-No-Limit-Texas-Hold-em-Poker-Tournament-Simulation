#pragma once
#include "AgentBehavior.h"
class RaiseBot : public AgentBehavior {
public:
	RaiseBot();

	int decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) override;
};

