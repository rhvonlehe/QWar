#pragma once

#include <Deck.h>
#include <Card.h>
#include <string>
#include <vector>

class Player
{
public:
    enum Pile
    {
        CURRENT,
        PLAYED
    };

    Player(void) = default;
    Player(const std::string name);
    void acceptNewCards(const Pile pile, const std::vector<std::shared_ptr<Card> > cards);
    void acceptNewCard(const Pile pile, const std::shared_ptr<Card> card);
    std::shared_ptr<Card> playCard(void);

    uint8_t totalPlayed(void) const
    {
        return _playedPile.size();
    }
    uint8_t totalUnplayed(void) const
    {
        return _unplayedPile.size();
    }

    bool hasTwoCards(void) const
    {
        return ((_unplayedPile.size() + _playedPile.size()) >= 2);
    }
    bool outOfCards(void) const
    {
        return (_unplayedPile.isEmpty() && _playedPile.isEmpty());
    }
    std::string name(void) const
    {
        return _name;
    }
    void name(std::string n)  { _name = n; }
    bool operator==(const Player& rhs)
    {
        return (_name == rhs.name());
    }
private:
    void movePlayedToCurrent();

    std::string _name;
    Deck        _unplayedPile;
    Deck        _playedPile;
};

