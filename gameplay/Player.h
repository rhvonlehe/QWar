#pragma once

#include <Deck.h>
#include <Card.h>
#include <PlayerState.h>
#include <string>
#include <vector>
#include <memory>
#include <thread>

class PlayerSM;

class Player
{
public:
    enum Pile
    {
        CURRENT,
        PLAYED
    };

    Player(void) = delete;
    Player(const std::string name);
    ~Player(void);
    void acceptNewCards(const Pile pile, const std::vector<std::shared_ptr<Card> > cards);
    void acceptNewCard(const Pile pile, const std::shared_ptr<Card> card);
//    std::shared_ptr<Card> playCard(void); // todo remove
    void playCard(void);

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
    void movePlayedToCurrent();

#if 0 // todo remove
    void changeState(PlayerState* next);
    friend class PlayerState;
#endif

    std::string     _name;
    Deck            _unplayedPile;
    Deck            _playedPile;

    using FifoScheduler = boost::statechart::fifo_scheduler<>;
    FifoScheduler                   _scheduler;
    FifoScheduler::processor_handle _processor;
    std::unique_ptr<std::thread>    _processorThread;
//    PlayerState*    _playerState;
};

