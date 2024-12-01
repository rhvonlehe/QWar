#pragma once

#include <Deck.h>
#include <Card.h>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <functional>
#define ASIO_STANDALONE
#include <boost/asio.hpp>


namespace ba = boost::asio;

namespace gameplay {

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
        EV_PLAYER_ELIMINATED,
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
    void acceptRoundCards(const Pile pile, const std::vector<Card> cards);
    void acceptDealtCard(const Pile pile, const Card card);
    void action(void);
    Card& evalCard(void);
    void tie(void);
    void won();
    std::vector<Card> lost(void);
    void addObserverCallback(const std::function<void (Player::ObservableEvent)> callback);

    Card lastCardPlayed(void) const
    {
        assert(activeRoundCards_.size());
        return activeRoundCards_.back();
    }

    std::vector<Card> getActiveRoundCards(void)  { return activeRoundCards_; }

    uint8_t totalPlayed(void) const { return playedPile_.size(); }

    uint8_t totalUnplayed(void) const { return unplayedPile_.size(); }

    bool hasTwoCards(void) const { return ((unplayedPile_.size() + playedPile_.size()) >= 2); }

    bool outOfCards(void) const { return (unplayedPile_.isEmpty() && playedPile_.isEmpty()); }

    std::string name(void) const { return name_; }

    void name(std::string n)  { name_ = n; }

    bool operator==(const Player& rhs) { return (name_ == rhs.name()); }

private:
    Card getNextCard(void);
    void movePlayedToCurrent();

private:
    // "hidden friends", better than friend classes - see Klaus Iglberger C++ Software Design
    friend void flipCard(Player& player);
    friend void playCard(Player& player, Card::Face face);
    friend void setEvalCard(Player& player);
    friend void notifyObservers(Player& player, ObservableEvent event);
    friend void resetRoundData(Player& player);
    friend void startTimer(Player& player, const boost::posix_time::milliseconds ms);
    friend void cancelTimer(Player& player);

    std::string         name_;
    Deck                unplayedPile_;
    Deck                playedPile_;

    std::vector<Card>   activeRoundCards_;
    uint8_t             evalCard_;

    // Observer variable - simplified for just one observer
    std::vector<std::function<void(ObservableEvent)>> observerFuncs_;

    // StateChart variables
    using FifoScheduler = boost::statechart::fifo_scheduler<>;
    FifoScheduler                   scheduler_;
    FifoScheduler::processor_handle processor_;
    std::thread                     stateThread_;

    // Asio stuff
    std::unique_ptr<ba::executor_work_guard<ba::io_context::executor_type>>  work_;
    std::unique_ptr<ba::deadline_timer>                     timer_;
    ba::io_context                                          io_;
    std::thread                                             ioCtxThread_;
};


} // namespace gameplay
