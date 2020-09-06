#include "Deck.h"

Deck::Deck()
{

}

void Deck::shuffle(void)
{
    int r1;
    int r2;

    // Swap two cards from random positions in the deck N times
    //
    for (int i =0; i < 2000; i++)
    {
        r1 = rand() % _cards.size();
        r2 = rand() % _cards.size();

        swap(r1, r2);
    }
}

std::shared_ptr<Card> Deck::nextCard()
{
    auto card = _cards.front();
    _cards.pop_front();
    return card;
}

void Deck::addBack(const std::shared_ptr<Card> card)
{
    _cards.push_back(card);
}

void Deck::addBack(const std::vector<std::shared_ptr<Card>> cards)
{
    _cards.insert(_cards.end(), cards.begin(), cards.end());
//    for (auto& card : cards)
//    {
//        addBack(card);
//    }
}

void Deck::print(void) const
{
    for (auto card : _cards)
    {
        card->print();
    }
}

void Deck::swap(const int r1, const int r2)
{
    auto temp = _cards[r1];
    _cards[r1] = _cards[r2];
    _cards[r2] = temp;
}
