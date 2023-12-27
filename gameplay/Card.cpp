#include "Card.h"

#include <iostream>
#include <map>

#include <optional>


#if 0 // todo remove
class Simple
{
public:
    Simple() {};
    Simple(Simple&& other) : _data(std::exchange(other._data, {})) {};
    Simple(const Simple& other) = delete;
private:
    int _data;
};

std::optional<Simple> test(void)
{
    std::optional<Simple> s;
    return std::move(s);      // std::optional<Simple>()
}
#endif


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

Card::Card(Suit suit, Value value, bool faceDown)
    : _suit(suit), _value(value), _faceDown(faceDown)
{

}

void Card::print(void) const
{
    std::cout << this->str() << std::endl;
}

std::string Card::str() const
{
    std::string printable;

    if (_faceDown)
    {
        printable += "XX";
    }
    else
    {
        printable += valStr[_value];
        printable += suitStr[_suit];
    }

    return printable;
}

void Card::flip(bool faceDown)
{
    _faceDown = faceDown;
}
