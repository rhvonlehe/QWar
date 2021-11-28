#pragma once

#include "Player.h"
#include <boost/statechart/event.hpp>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>

#include <iostream>
#define TEMP_LOG(X) { std::cout << X << std::endl; }

// Declare things in one spot for visual convenience
//
// Player events
//
struct EvOutOfCards;
struct EvPlay;
struct EvFlip;
struct EvWinner;
struct EvWinnterTie;
struct EvLoser;

// Player states
//
struct Idle;
struct Eliminated;
struct CardsPlayed;
struct WaitingForWinner;
struct WaitFirstCard;
struct WaitHoleCard;
struct WaitLastCard;
struct WaitFlip;

struct PlayerSM : boost::statechart::asynchronous_state_machine<PlayerSM, Idle>
{
    PlayerSM(my_context ctx, Player& player)
        : my_base(ctx),
          _player(player) {}
    ~PlayerSM() { terminate(); }



private:
    Player&   _player;
};

// More definition for events
//
struct EvOutOfCards : boost::statechart::event < EvOutOfCards >
{
    EvOutOfCards() { TEMP_LOG("EvOutOfCards event"); }
};
struct EvPlay       : boost::statechart::event < EvPlay >
{
    EvPlay() { TEMP_LOG("EvPlay event"); }
};
struct EvFlip       : boost::statechart::event < EvFlip >
{
    EvFlip() { TEMP_LOG("EvFlip event"); }
};
struct EvWinner     : boost::statechart::event < EvWinner>
{
    EvWinner() { TEMP_LOG("EvWinner event"); }
};
struct EvWinnerTie  : boost::statechart::event < EvWinnerTie >
{
    EvWinnerTie() { TEMP_LOG("EvWinnerTie event"); }
};
struct EvLoser      : boost::statechart::event < EvLoser >
{
    EvLoser() { TEMP_LOG("EvLoser event"); }
};

// More definition for states
//
struct Idle : boost::statechart::state<Idle, PlayerSM>
{
    typedef boost::mpl::list<
    boost::statechart::transition< EvPlay, CardsPlayed > > reactions;

    Idle(my_context ctx);
    ~Idle();
};

struct Eliminated : boost::statechart::state<Eliminated, PlayerSM>
{
    Eliminated(my_context ctx);
    ~Eliminated();
};

struct CardsPlayed : boost::statechart::state<CardsPlayed, PlayerSM>
{
    typedef boost::mpl::list<
    boost::statechart::transition< EvOutOfCards, Eliminated >,
    boost::statechart::transition< EvLoser, Idle>,
    boost::statechart::transition< EvWinner, Idle> > reactions;

    CardsPlayed(my_context ctx);
    ~CardsPlayed();
};



