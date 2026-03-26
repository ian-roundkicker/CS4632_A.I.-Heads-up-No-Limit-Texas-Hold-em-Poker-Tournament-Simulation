#pragma once
#include "AgentBehavior.h"
class WorstBot : public AgentBehavior{
public:
	WorstBot();
	int decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) override;
};

