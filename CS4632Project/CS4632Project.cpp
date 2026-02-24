// CS4632Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// this is here because it is here by default
#include <iostream>
// this is here so that I can use std::shuffle
#include <algorithm>
// std::random_device and std::mt19937
#include <random>
// for std::begin() and std::end()
#include <iterator>
//phevaluator library
#include <phevaluator/phevaluator.h>
#include <phevaluator/card.h>
#include <phevaluator/card_sampler.h>
#include <phevaluator/rank.h>

class Card {
public:
    int card_num;

    // default constructor exists so that I can assign array elements
    Card() {
        card_num = -1;
    }

    Card(int num) {
        card_num = num;
    }

    std::string getName() {
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

    
};

// Deck class represents a full deck of cards.
class Deck {
public:
    Card deck[52];
    std::random_device rd = std::random_device();
    std::mt19937 mt = std::mt19937(rd());

    Deck() {
        for (int i = 0; i < 52; i++) {
            deck[i] = Card(i);
        }
    }

    // shuffles the cards
    void Shuffle() {
        std::shuffle(std::begin(deck), std::end(deck), mt);
    }
};

class PlayerHand {
public:
    std::vector<Card*> cards;

    PlayerHand() {
        cards = std::vector<Card*>();
    }

    //todo: throw exception if size > 5
    void give_card(Card* c) {
        if (cards.size() < 5) {
            cards.push_back(c);
        }
    }

    void clear_cards() {
        cards.clear();
    }

    void sort() {
       std::sort(std::begin(cards), std::end(cards), [](const Card* a, const Card* b) {return a->card_num < b->card_num;});
    }

    void print_hand() {
        for (Card* c : cards) {
            if (c == nullptr) {
                return;
            }
            std::cout << c->getName() << "\n";
        }
    }

    int getRank() {
        switch (cards.size()) {
        case 5:
            return evaluate_5cards(cards.at(0)->card_num, cards.at(1)->card_num, cards.at(2)->card_num, cards.at(3)->card_num, cards.at(4)->card_num);
        default:
            throw std::exception("Invalid number of cards to get rank.");
        }
        
    }

};
int main()
{
    Deck d1 = Deck();
    std::cout << "Sorted deck:" << "\n";
    for (Card c : d1.deck) {
        std::cout << c.getName() << "\n";
    }
    std::cout << "\n\nUnsorted deck:" << "\n";
    d1.Shuffle();
    for (Card c : d1.deck) {
        std::cout << c.getName() << "\n";
    }
    PlayerHand p1 = PlayerHand();
    for (int i = 0; i < 5; i++) {
        p1.give_card(&d1.deck[i]);
    }
    p1.sort();
    std::cout << "\n\nPlayer 1 hand:" << "\n";
    p1.print_hand();
    PlayerHand p2 = PlayerHand();
    for (int i = 5; i < 10; i++) {
        p2.give_card(&d1.deck[i]);
    }
    p2.sort();
    std::cout << "\n\nPlayer 2 hand:" << "\n";
    p2.print_hand();
    std::string winner = p1.getRank() < p2.getRank() ? "Player 1 wins!" : "Player 2 wins!";
    std::cout << winner << "\n";
    std::cout << "END OF PROGRAM";
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
