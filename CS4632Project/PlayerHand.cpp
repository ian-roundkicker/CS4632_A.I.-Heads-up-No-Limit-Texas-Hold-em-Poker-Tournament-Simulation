#include "PlayerHand.h"

//returns a unique pointer to an AgentBehavior object based on the type parameter passed in. This is used to determine the Agent Used
static std::unique_ptr<AgentBehavior> choose_type(int type) {
    switch (type)
    {
    case 0:
        return std::make_unique<A_RandomAction>();
    case 1:
        return std::make_unique<WeightedRandom>();
    case 2:
		return std::make_unique<WorstBot>();
    case 3:
		return std::make_unique<RaiseBot>();
    case 4:
		return std::make_unique<Cautious>();
    default:
        break;
    }
}

PlayerHand::PlayerHand(int player_id, int type) : behavior(choose_type(type)), player_id(player_id) {
    cards = std::vector<Card*>();
    available_chips = 5;
}


void PlayerHand::give_card(Card* c) {
    if (cards.size() < 5) {
        cards.push_back(c);
    }
    else {
        throw std::exception("Invalid number of cards to get rank.");
    }
}

void PlayerHand::clear_cards() {
    cards.clear();
}

void PlayerHand::sort() {
    std::sort(std::begin(cards), std::end(cards), [](const Card* a, const Card* b) {return a->card_num < b->card_num;});
}

void PlayerHand::print_hand() {
    for (Card* c : cards) {
        if (c == nullptr) {
            return;
        }
        std::cout << c->getName() << "\n";
    }
}

int PlayerHand::getRank(std::array<Card*, 5> community_cards) {
    return evaluate_7cards(cards.at(0)->card_num, cards.at(1)->card_num, community_cards.at(0)->card_num, community_cards.at(1)->card_num, community_cards.at(2)->card_num, community_cards.at(3)->card_num, community_cards.at(4)->card_num);
}



