#pragma once

#include <Player.h>
#include <Deck.h>
#include <Round.h>
#include <vector>
#include <map>


class Game
{
public:
    Game(std::vector<std::string>& playerNames);
#if 0
    void autoPlay();
#endif

    Player& getPlayer(const std::string name);

//    std::vector<Player>& players(void) {return _activePlayers; } // todo reimplement or remove

    bool isOver(void) const
    {
        return (_activePlayers.size() == 1);
    }

    // Deal is similar to 'init' or 'reset' on the game
    void deal();

    void handleRoundComplete(void);

    void handlePlayerUpdate(Player *player,
                            Player::ObservableEvent event);

private:

    std::vector<Player*>                            _activePlayers;
    std::map<std::string, std::unique_ptr<Player>>  _allPlayers;
    std::unique_ptr<Deck>                           _deck;
    std::unique_ptr<Round>                          _round;
    uint32_t                                        _roundNumber;
};

