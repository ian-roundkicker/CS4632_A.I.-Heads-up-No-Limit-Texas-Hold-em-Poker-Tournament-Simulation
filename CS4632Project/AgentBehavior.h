#pragma once
#include <vector>
#include "Card.h"
class AgentBehavior
{
	public:
	// This is the base class for all agent behaviors. It defines the interface for decision-making.
	virtual ~AgentBehavior() = default;

	// This function will be called to determine the agent's action during their turn.
	// -1 to fold, 0 to call, and any number greater than 0 to raise by that amount.
	virtual int decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) = 0;
};

