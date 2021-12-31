#pragma once

#include <Player.h>
#include <Deck.h>
#include <Round.h>
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

    // Deal is similar to 'init' or 'reset' on the game
    void deal();

    void handlePlayerUpdate(std::shared_ptr<Player> player,
                            Player::ObservableEvent event);

private:
    void cullPlayerList();

    std::vector<std::shared_ptr<Player>>    _activePlayers;
    std::vector<std::shared_ptr<Player>>    _allPlayers;
    std::unique_ptr<Deck>                   _deck;
    std::unique_ptr<Round>                  _round;
    uint32_t                                _roundNumber;
};

