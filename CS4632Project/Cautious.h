#pragma once
#include "AgentBehavior.h"
#include <phevaluator/phevaluator.h>
class Cautious : public AgentBehavior{
public:
	Cautious();
	int decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) override;
};

