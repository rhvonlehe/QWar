#pragma once

#include <Card.h>
#include <deque>
#include <vector>
#include <memory>

class Deck
{
public:
    Deck();
    void shuffle(void);
    std::shared_ptr<Card> nextCard(void);
    void addBack(const std::shared_ptr<Card> card);
    void addBack(const std::vector<std::shared_ptr<Card>> cards);
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

    std::deque<std::shared_ptr<Card>> _cards;
};

