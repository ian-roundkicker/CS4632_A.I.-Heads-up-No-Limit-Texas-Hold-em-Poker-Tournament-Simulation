#include "PlayerHand.h"

PlayerHand::PlayerHand(int player_id) : behavior() {
    cards = std::vector<Card*>();
    this->player_id = player_id;
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

