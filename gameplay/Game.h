#pragma once

#include <Player.h>
#include <Deck.h>
#include <vector>


class Game
{
public:
    Game(std::vector<Player>& players);

    void play();

    std::vector<Player>& players(void) {return _activePlayers; }

    bool isOver(void) const
    {
        return (_activePlayers.size() == 1);
    }

private:
    void initDeck();
    void deal();
    void cullPlayerList();

    std::vector<Player> _activePlayers;
    Deck                _deck;
    int                 _roundNumber;
};

