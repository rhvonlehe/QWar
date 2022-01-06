#pragma once

#include <Deck.h>
#include <Card.h>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <map>
#include <functional>

class PlayerSM;

class Player
{
public:
    enum Pile
    {
        UNPLAYED,
        PLAYED
    };
    enum ObservableEvent
    {
        EV_PLAYER_WAITING,
        EV_PLAYER_ACTIVE,
        EV_CARD_PLAYED,
        EV_CARDS_CHANGED,
        EV_WINNER,
        EV_LOSER,
        EV_TIE
    };

    Player(void) = delete;
    Player(const std::string name);
    ~Player(void);
    void reset(void);
    void acceptNewCards(const Pile pile, const std::vector<std::shared_ptr<Card> > cards);
    void acceptNewCard(const Pile pile, const std::shared_ptr<Card> card);
    void playCard(void);
    std::shared_ptr<Card> evalCard(void);
    void tie(void);
    void won(void);
    void lost(std::shared_ptr<Player> winner);
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
    std::shared_ptr<Card> getNextCard(void);
    void setEvalCard(void);
    void endRound(void);

    friend class PlayerSM;
    void movePlayedToCurrent();
    void notifyEvent(ObservableEvent event);

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
    std::unique_ptr<std::thread>    _processorThread;
};

