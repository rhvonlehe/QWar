#pragma once

#include <Player.h>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <vector>
#include <map>
#include <memory>

class Round
{
public:
    Round(std::vector<Player*>& players,
          const std::function<void(void)> callback);

    ~Round(void);

    void playerWaiting(Player* player);
    void playerEliminated(Player* player);
    void winnerReqCards(Player* player);

private:
    friend class RoundSM;
    friend class Done;
    friend class Active;
    void handlePlayerWaiting(Player *player);
    void handlePlayerEliminated(Player* player);
    void evaluate(void);
    void findWinner(void);
    void cullLoserList(void);
    void initializeRound(void);
    void distributeCards(Player *winner);

    int                             _playersWaiting;
    std::vector<Player*>            _losers;
    std::vector<Player*>            _players;

    std::function<void()>           _observerFunc;

    // StateChart variables
    using FifoScheduler = boost::statechart::fifo_scheduler<>;
    FifoScheduler                   _scheduler;
    FifoScheduler::processor_handle _processor;
    std::unique_ptr<std::thread>    _processorThread;

};
