#pragma once
#include <iostream>
// represents a single card
class Card {
public:
    int card_num;

    Card();
    Card(int num);
    std::string getName();
};
