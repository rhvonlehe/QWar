#pragma once

#include <Deck.h>
#include <Card.h>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <map>
#include <functional>

namespace ba = boost::asio;

class PlayerSM;

class Player
{
public:
    enum Pile
    {
        PILE_UNPLAYED,
        PILE_PLAYED
    };
    enum ObservableEvent
    {
        EV_PLAYER_WAITING,
        EV_PLAYER_ACTIVE,
        EV_WINNER_REQ_CARDS,
        EV_CARD_PLAYED,
        EV_CARD_FLIPPED,
        EV_CARDS_CHANGED,
        EV_WINNER,
    };

    Player(void) = delete;
    Player(const std::string name);
    ~Player(void);
    void reset(void);
    void acceptRoundCards(const Pile pile, const std::vector<std::shared_ptr<Card> > cards);
    void acceptDealtCard(const Pile pile, const std::shared_ptr<Card> card);
    void action(void);
    std::shared_ptr<Card> evalCard(void);
    void tie(void);
    void won();
    std::vector<std::shared_ptr<Card> > lost(void);
    void addObserverCallback(const std::function<void (Player::ObservableEvent)> callback);

    std::shared_ptr<Card> lastCardPlayed(void) const
    {
        assert(_activeRoundCards.size());
        return _activeRoundCards.back();
    }

    std::vector<std::shared_ptr<Card>> getActiveRoundCards(void)
    {
        return _activeRoundCards;
    }

    uint8_t totalPlayed(void) const
    {
        return _playedPile.size();
    }
    uint8_t totalUnplayed(void) const
    {
        return _unplayedPile.size();
    }

    bool hasTwoCards(void) const
    {
        return ((_unplayedPile.size() + _playedPile.size()) >= 2);
    }
    bool outOfCards(void) const
    {
        return (_unplayedPile.isEmpty() && _playedPile.isEmpty());
    }
    std::string name(void) const
    {
        return _name;
    }
    void name(std::string n)  { _name = n; }
    bool operator==(const Player& rhs)
    {
        return (_name == rhs.name());
    }
private:
    bool playCard(bool faceDown = false);
    void flipCard(void);
    std::shared_ptr<Card> getNextCard(void);
    void setEvalCard(void);
    void resetRoundData(void);
    void movePlayedToCurrent();
    void notifyEvent(ObservableEvent event);
    void startTimer(const boost::posix_time::milliseconds ms);
    void cancelTimer(void);

private:
    friend class PlayerSM;
    friend class Idle;
    friend class WaitHoleCard;
    friend class WaitLastCard;
    friend class WaitFlip;
    friend class WaitForWinner;
    friend class AcceptingCards;

    std::string         _name;
    Deck                _unplayedPile;
    Deck                _playedPile;

    std::vector<std::shared_ptr<Card>>  _activeRoundCards;
    std::shared_ptr<Card>               _evalCard;

    // Observer variable - simplified for just one observer
    std::vector<std::function<void(ObservableEvent)>> _observerFuncs;

    // StateChart variables
    using FifoScheduler = boost::statechart::fifo_scheduler<>;
    FifoScheduler                   _scheduler;
    FifoScheduler::processor_handle _processor;
    std::thread                     _processorThread;

    // Asio stuff
    std::unique_ptr<ba::executor_work_guard<ba::io_context::executor_type>>  _work;
    std::unique_ptr<ba::deadline_timer>                     _timer;
    ba::io_context                                          _io;
    std::thread                                             _asioThread;
};

