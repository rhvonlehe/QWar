#pragma once

#include <string>

namespace gameplay {

class Card
{
public:
    enum Face
    {
        FACEDOWN,
        FACEUP
    };
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
    Card(Suit suit, Value value, Face face);

    inline Suit suit(void) const
    {
        return suit_;
    }

    inline Value value(void) const
    {
        return value_;
    }

    bool operator==(Card& rhs) const
    {
        return (value_ == rhs.value_);
    }

    bool operator>(Card& rhs) const
    {
        return (value_ > rhs.value_);
    }

    bool operator<(Card& rhv) const
    {
        return (value_ < rhv.value_);
    }

    void print() const;

    std::string str(void) const;

    void flip(Face face);
private:

    Suit  suit_;
    Value value_;
    Face  face_;
};

} // gameplay
