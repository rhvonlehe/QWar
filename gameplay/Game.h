#pragma once

#include <Player.h>
#include <Deck.h>
#include <Round.h>
#include <EventScheduler.h>
#include <vector>
#include <map>

namespace gameplay {

class Game
{
public:
    Game(std::vector<std::string>& playerNames);
#if 0
    void autoPlay();
#endif

    Player& getPlayer(const std::string name);

//    std::vector<Player>& players(void) {return activePlayers_; } // todo reimplement or remove

    bool isOver(void) const
    {
        return (activePlayers_.size() == 1);
    }

    // Deal is similar to 'init' or 'reset' on the game
    void deal();

    void handleRoundComplete(void);

    void handlePlayerUpdate(Player *player,
                            Player::ObservableEvent event);

private:

    std::vector<Player*>                            activePlayers_;
    std::map<std::string, std::unique_ptr<Player>>  allPlayers_;
    std::unique_ptr<Deck>                           deck_;
    std::unique_ptr<Round>                          round_;
    uint32_t                                        roundNumber_;

    EventScheduler                                  eventScheduler_;
};

}
