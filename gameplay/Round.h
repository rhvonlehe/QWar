#pragma once

#include <Player.h>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <vector>
#include <map>
#include <memory>

struct WarHand
{
    WarHand() = default;
    WarHand(std::shared_ptr<Player> p, std::shared_ptr<Card> c1, std::shared_ptr<Card> c2)
        : player(p), downCard(c1), upCard(c2)
    {
    }
    std::shared_ptr<Player> player;
    std::shared_ptr<Card>   downCard;
    std::shared_ptr<Card>   upCard;
};


class Round
{
public:
    Round(std::vector<std::shared_ptr<Player>>& players);

    ~Round(void);

    void sendPlayerWaiting(std::shared_ptr<Player> player);


#if 0 // todo
    void play();
//    bool playNormal(std::shared_ptr<Player>& player); // todo
    std::vector<std::shared_ptr<Player>> playNormal();
    std::vector<std::shared_ptr<Player> > playWar(std::vector<std::shared_ptr<Player> > &players);
#endif

    const std::vector<std::shared_ptr<Player>>& winners(void) const;
private:
    friend class RoundSM;
    void handlePlayerWaiting(std::shared_ptr<Player> player);
     bool allPlayersWaiting(void) const;

    std::vector<std::shared_ptr<Player>> findWinner(std::vector<std::pair<std::shared_ptr<Player>,
                                    std::shared_ptr<Card>>>& played);
    std::vector<std::shared_ptr<Player> > findWinner(std::vector<WarHand>& played);
    void removePlayer(std::shared_ptr<Player> player);

    std::vector<std::shared_ptr<Player>>    _playersActive;
    std::vector<std::shared_ptr<Player>>    _winners;
    std::vector<std::shared_ptr<Player>>    _players;

    // StateChart variables
    using FifoScheduler = boost::statechart::fifo_scheduler<>;
    FifoScheduler                   _scheduler;
    FifoScheduler::processor_handle _processor;
    std::unique_ptr<std::thread>    _processorThread;

};
