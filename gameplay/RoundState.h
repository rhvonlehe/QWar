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

namespace gameplay {

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

    Round& _round;
};

// More definition for events
//
struct EvWinner : sc::event < EvWinner >
{
    EvWinner(Player* player);
    ~EvWinner(void) {};

    Player* player;
};

struct EvPlayerWaiting : sc::event < EvPlayerWaiting >
{
    EvPlayerWaiting(Player* player)
        : player(player){ TEMP_LOG("EvPlayerWaiting event"); }

    Player* player;
};

struct EvPlayerEliminated : sc::event < EvPlayerEliminated >
{
    EvPlayerEliminated(Player* player)
        : player(player) { TEMP_LOG("EvPlayerEliminated event"); }

    Player* player;
};

struct EvDistributeCards : sc::event < EvDistributeCards >
{
    EvDistributeCards(Player* player)
        : player(player) { TEMP_LOG("EvDistributeCards event"); }

    Player* player;
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
    sc::result react(const EvPlayerEliminated& event);
    sc::result react(const EvWinner& event);
};

struct Done : sc::state<Done, RoundSM>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvDistributeCards>,
    sc::custom_reaction< EvPlayerEliminated> > reactions;

    Done(my_context ctx);
    ~Done(void);

    sc::result react(const EvDistributeCards& event);
    sc::result react(const EvPlayerEliminated& event);
};

} // gameplay
