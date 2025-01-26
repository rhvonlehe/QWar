#pragma once

#include <Player.h>
#include <EventScheduler.h>
// #include <boost/statechart/asynchronous_state_machine.hpp> // todo remove
#include <vector>

namespace gameplay {

class Round
{
public:
    enum ObservableEvent
    {
        EV_ROUND_COMPLETE,
        EV_ROUND_ONE_PLAYER_LEFT
    };

    Round(std::vector<Player*>& players,
          EventScheduler& scheduler,
          const std::function<void (ObservableEvent)> callback);

    ~Round(void);

    void playerWaiting(Player* player);
    void playerEliminated(Player* player);
    void winnerReqCards(Player* player);
    int  activePlayers(void) const;

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

    int                             playersWaiting_;
    std::vector<Player*>            losers_;
    std::vector<Player*>            players_;

    std::function<void(Round::ObservableEvent event)>   observerFunc_;

    EventScheduler&                 scheduler_;
    ProcessorHandle                 procHandle_;


#if 0   // todo remove
    // StateChart variables
    using FifoScheduler = boost::statechart::fifo_scheduler<>;
    FifoScheduler                   _scheduler;
    FifoScheduler::processor_handle _processor;
    std::unique_ptr<std::thread>    _processorThread;
#endif

};

} // gameplay
