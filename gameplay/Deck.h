#pragma once

#include <Card.h>
#include <deque>
#include <vector>

class Deck
{
public:
    Deck();
    void shuffle(void);
    Card nextCard(void);
    void addBack(const Card& card);
    void addBack(const std::vector<Card> cards);
    void print(void) const;
    int size(void) const
    {
        return static_cast<int>(_cards.size());
    }
    bool isEmpty(void) const
    {
        return (0 == _cards.size());
    }

private:
    void swap(const int r1, const int r2);

    std::deque<Card> _cards;
};

