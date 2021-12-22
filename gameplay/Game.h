#pragma once

#include <Player.h>
#include <Deck.h>
#include <vector>
#include <map>


class Game
{
public:
    Game(std::vector<std::shared_ptr<Player>>& players);

#if 0
    void autoPlay();
#endif

    std::vector<std::shared_ptr<Player>>& players(void) {return _activePlayers; }

    bool isOver(void) const
    {
        return (_activePlayers.size() == 1);
    }

    void deal();

private:
    void initDeck();
    void cullPlayerList();

    std::vector<std::shared_ptr<Player>>    _activePlayers;
    std::unique_ptr<Deck>                   _deck;
    int                                     _roundNumber;
};

