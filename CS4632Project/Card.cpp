#include "Card.h"
// default constructor exists so that I can use an Array for the card
Card::Card() {
    card_num = -1;
}

// overloaded constructor is what is primarily used
Card::Card(int num) {
    card_num = num;
}

// returns a string representing the name and suit of the card
std::string Card::getName() {
    std::string name = "";
    std::string suit = "";
    int card_category = card_num / 4;
    switch (card_category) {
    case 0:
        name = "2";
        break;
    case 1:
        name = "3";
        break;
    case 2:
        name = "4";
        break;
    case 3:
        name = "5";
        break;
    case 4:
        name = "6";
        break;
    case 5:
        name = "7";
        break;
    case 6:
        name = "8";
        break;
    case 7:
        name = "9";
        break;
    case 8:
        name = "10";
        break;
    case 9:
        name = "Judge";
        break;
    case 10:
        name = "Queen";
        break;
    case 11:
        name = "King";
        break;
    case 12:
        name = "Ace";
        break;
    }
    card_category = card_num % 4;
    switch (card_category) {
    case 0:
        suit = "Clubs";
        break;
    case 1:
        suit = "Diamonds";
        break;
    case 2:
        suit = "Hearts";
        break;
    case 3:
        suit = "Spades";
        break;
    }
    return name + " of " + suit;
}