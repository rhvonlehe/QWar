#ifndef PLAYER_H
#define PLAYER_H

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

    Player(const std::string name);
    void acceptNewCards(const Pile pile, const std::vector<Card>& cards);
    void acceptNewCard(const Pile pile, const Card card);
    Card playCard(void);
    bool hasTwoCards(void) const
    {
        return ((_currentDeck.size() + _playedDeck.size()) >= 2);
    }
    bool outOfCards(void) const
    {
        return _currentDeck.isEmpty();
    }
    std::string name(void) const
    {
        return _name;
    }
    bool operator==(const Player& rhs)
    {
        return (_name == rhs.name());
    }
private:
    void movePlayedToCurrent();

    std::string _name;
    Deck        _currentDeck;
    Deck        _playedDeck;



};

#endif // PLAYER_H
