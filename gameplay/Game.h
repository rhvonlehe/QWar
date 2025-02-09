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
    enum ObservableEvent
    {
        EV_GAME_FINISHED
    };

    Game(std::vector<std::string>& playerNames);

    Player* getPlayer(const std::string name);

//    std::vector<Player>& players(void) {return activePlayers_; } // todo reimplement or remove

    void addObserverCallback(const std::function<void (Game::ObservableEvent)> callback);

    bool isFinished(void) const
    {
        return (round_->activePlayers() == 1);
    }

    // Deal is similar to 'init' or 'reset' on the game
    void deal();

private:
    void handleRoundComplete(Round::ObservableEvent event);

    void handlePlayerUpdate(Player *player,
                            Player::ObservableEvent event);

    void notifyObservers(Game::ObservableEvent event);


    std::vector<Player*>                            activePlayers_;
    std::map<std::string, std::unique_ptr<Player>>  allPlayers_;
    std::unique_ptr<Deck>                           deck_;
    std::unique_ptr<Round>                          round_;
    uint32_t                                        roundNumber_;

    EventScheduler                                  eventScheduler_;

    std::vector<std::function<void(ObservableEvent)>> observerFuncs_;
};

}
