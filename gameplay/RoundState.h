#pragma once

#include <boost/statechart/event.hpp>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

#include <iostream>
#include <memory>

#define TEMP_LOG(X) { std::cout << X << std::endl; }

namespace sc = boost::statechart;

class Round;
class Player;

// Round events
//
struct EvWinner;
struct EvPlayerWaiting;
struct EvDistributeCards;

// Round State
//
struct Active;
struct Done;

struct RoundSM : sc::asynchronous_state_machine<RoundSM, Active>
{
    RoundSM(my_context ctx, Round& round);
    ~RoundSM(void) { terminate(); }

    // common stuff done in state machine context since it has Round&
    void handlePlayerWaiting(std::shared_ptr<Player> player);
    void initializeRound(void);
    void distributeCards(std::shared_ptr<Player> player);
private:
    Round& _round;
};

// More definition for events
//
struct EvWinner : sc::event < EvWinner >
{
    EvWinner(std::shared_ptr<Player> player);
    ~EvWinner(void) {};

    std::shared_ptr<Player> player;
};

struct EvPlayerWaiting : sc::event < EvPlayerWaiting >
{
    EvPlayerWaiting(std::shared_ptr<Player> player)
        : player(player){ TEMP_LOG("EvPlayerWaiting event"); }

    std::shared_ptr<Player> player;
};

struct EvDistributeCards : sc::event < EvDistributeCards >
{
    EvDistributeCards(std::shared_ptr<Player> player)
        : player(player) { TEMP_LOG("EvDistributeCards event"); }

    std::shared_ptr<Player> player;
};


// More definition for states
//
struct Active : sc::state<Active, RoundSM>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvPlayerWaiting >,
    sc::custom_reaction< EvWinner > > reactions;

    Active(my_context ctx);
    ~Active(void);

    sc::result react(const EvPlayerWaiting& event);
    sc::result react(const EvWinner& event);
};

struct Done : sc::state<Done, RoundSM>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvDistributeCards> > reactions;

    Done(my_context ctx);
    ~Done(void);

    sc::result react(const EvDistributeCards& event);
};
