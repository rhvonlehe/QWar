#pragma once

#include <string>

class Card
{
public:
    enum Suit
    {
        SPADES,
        CLUBS,
        DIAMONDS,
        HEARTS
    };

    enum Value
    {
        TWO = 2,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE
    };

    Card() = default;
    Card(Suit suit, Value value);

    inline Suit suit(void) const
    {
        return _suit;
    }

    inline Value value(void) const
    {
        return _value;
    }

    bool operator==(Card& rhs) const
    {
        return (_value == rhs._value);
    }

    bool operator>(Card& rhs) const
    {
        return (_value > rhs._value);
    }

    bool operator<(Card& rhv) const
    {
        return (_value < rhv._value);
    }

    void print() const;

    std::string str(void) const;


private:

    Suit  _suit;
    Value _value;
};

