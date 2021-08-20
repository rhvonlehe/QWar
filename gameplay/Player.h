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
//    Player& operator=(Player&& other);
    void acceptNewCards(const Pile pile, const std::vector<std::shared_ptr<Card> > cards);
    void acceptNewCard(const Pile pile, const std::shared_ptr<Card> card);
    std::shared_ptr<Card> playCard(void);

    uint8_t totalPlayed(void) const
    {
        return _playedCards.size();
    }
    uint8_t totalUnplayed(void) const
    {
        return _unplayedCards.size();
    }

    bool hasTwoCards(void) const
    {
        return ((_unplayedCards.size() + _playedCards.size()) >= 2);
    }
    bool outOfCards(void) const
    {
        return (_unplayedCards.isEmpty() && _playedCards.isEmpty());
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
    Deck        _unplayedCards;
    Deck        _playedCards;
};

