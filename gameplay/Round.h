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
    Round(std::vector<std::shared_ptr<Player>>& players,
          const std::function<void(void)> callback);

    ~Round(void);

    void playerWaiting(std::shared_ptr<Player> player);
    void winnerReqCards(std::shared_ptr<Player> player);


#if 0 // todo
    std::vector<std::shared_ptr<Player>> playNormal();
    std::vector<std::shared_ptr<Player> > playWar(std::vector<std::shared_ptr<Player> > &players);
#endif

    const std::vector<std::shared_ptr<Player>>& winners(void) const;
private:
    friend class RoundSM;
    void handlePlayerWaiting(std::shared_ptr<Player> player);
    void evaluate(void);

    void findWinner(void);

    std::vector<std::shared_ptr<Player>>    _losers;
    std::vector<std::shared_ptr<Player>>    _players;

    std::function<void()>                   _observerFunc;

    // StateChart variables
    using FifoScheduler = boost::statechart::fifo_scheduler<>;
    FifoScheduler                   _scheduler;
    FifoScheduler::processor_handle _processor;
    std::unique_ptr<std::thread>    _processorThread;

};
