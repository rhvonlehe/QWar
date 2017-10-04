#ifndef GAME_H
#define GAME_H

#include <Player.h>
#include <Deck.h>
#include <vector>


class Game
{
public:
    Game(std::vector<Player>& players);

    void play();

    bool isOver(void) const
    {
        return (_activePlayers.size() == 1);
    }

private:
    void initDeck();
    void deal();

    std::vector<Player> _activePlayers;
    Deck                _deck;
    int                 _roundNumber;
};

#endif // GAME_H
