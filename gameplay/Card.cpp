#include "Card.h"

#include <iostream>
#include <map>

#include <optional>

namespace gameplay {


static std::map<Card::Suit, char> suitStr
{
    { Card::SPADES, 'S' /*"0x2660"*/ },
    { Card::CLUBS, 'C' /*0x2665*/ },
    { Card::DIAMONDS, 'D' /*0x2662*/ },
    { Card::HEARTS, 'H' /*0x2667*/ }
};

static std::map<Card::Value, char> valStr
{
    {Card::TWO,   '2'},
    {Card::THREE, '3'},
    {Card::FOUR,  '4'},
    {Card::FIVE,  '5'},
    {Card::SIX,   '6'},
    {Card::SEVEN, '7'},
    {Card::EIGHT, '8'},
    {Card::NINE,  '9'},
    {Card::TEN,   'T'},
    {Card::JACK,  'J'},
    {Card::QUEEN, 'Q'},
    {Card::KING,  'K'},
    {Card::ACE,   'A'}
};

Card::Card(Suit suit, Value value, Face face)
    : suit_(suit), value_(value), face_(face)
{
}

void Card::print(void) const
{
    std::cout << this->str() << std::endl;
}

std::string Card::str() const
{
    std::string printable;

    if (FACEDOWN)
    {
        printable += "XX";
    }
    else
    {
        printable += valStr[value_];
        printable += suitStr[suit_];
    }

    return printable;
}

void Card::flip(Face face)
{
    face_ = face;
}

} // gameplay
