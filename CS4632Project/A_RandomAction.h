#pragma once
// AgentBehavior.h is the base class for all agent behaviors, with this incluse statement comes Card.h and vector
#include "AgentBehavior.h"
#include <random>
#include <array>
//todo: make this inherit from AgentBehavior
class A_RandomAction : public AgentBehavior {
	public:
		std::mt19937 mt;
		
		A_RandomAction();


	// This agent will randomly choose to fold, call, or raise during its turn.
		int decideAction(std::vector<Card*>* own_cards, std::array<Card*, 5>* community_cards, int current_bet, int current_chips) override;


};