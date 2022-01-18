#include "Deck.h"
#include <cassert>
#include <algorithm>
#include <array>

const std::array<Card::Suit, 4> cardSuits
{
    Card::SPADES, Card::CLUBS, Card::DIAMONDS, Card::HEARTS
};

const std::array<Card::Value, 13> cardValues
{
    Card::TWO, Card::THREE, Card::FOUR, Card::FIVE, Card::SIX, Card::SEVEN,
            Card::EIGHT, Card::NINE, Card::TEN, Card::JACK, Card::QUEEN,
            Card::KING, Card::ACE
};

Deck::Deck(bool full)
{
    // initialize to full deck?
    if (full)
    {
        std::for_each(cardValues.begin(), cardValues.end(), [&](const Card::Value& value) {
            std::for_each(cardSuits.begin(), cardSuits.end(), [&](const Card::Suit& suit) {
                _cards.push_back(std::make_shared<Card>(suit, value));
            });
        }
        );
    }
}

void Deck::clear(void)
{
    _cards.clear();
}

void Deck::shuffle(void)
{
    assert(_cards.size());

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
}

void Deck::print(void) const
{
    std::for_each(_cards.begin(), _cards.end(), [](const std::shared_ptr<Card>& card) { card->print(); });
}

void Deck::swap(const int r1, const int r2)
{
    auto temp = _cards[r1];
    _cards[r1] = _cards[r2];
    _cards[r2] = temp;
}
