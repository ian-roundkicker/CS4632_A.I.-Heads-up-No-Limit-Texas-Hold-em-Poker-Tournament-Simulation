#pragma once
// commenting this out for a bit
#include "AgentBehavior.h"
#include <random>
#include <vector>
#include <array>
//todo: make this inherit from AgentBehavior
class A_RandomAction {
	public:
		std::mt19937 mt;
		
		A_RandomAction();

	// This agent will randomly choose to fold, call, or raise during its turn.
		int decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips);
};