#pragma once

#include "Player.h"
#include <boost/statechart/event.hpp>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

#include <iostream>
#define TEMP_LOG(X) { std::cout << X << std::endl; }

namespace sc = boost::statechart;

// Player events
//
struct EvAction;
struct EvFlip;
struct EvWon;
struct EvTie;
struct EvLost;
struct EvAction;
struct EvAcceptCards;
struct EvReset;
struct EvTimeout;

// Player states
//
struct Idle;
struct Eliminated;
struct AcceptingCards;
struct CardsPlayed;
struct WaitForWinner;
struct WaitHoleCard;
struct WaitLastCard;
struct WaitFlip;

struct PlayerSM : sc::asynchronous_state_machine<PlayerSM, Idle>
{
    PlayerSM(my_context ctx, Player& player)
        : my_base(ctx),
          _player(player) {}
    ~PlayerSM(void) { terminate(); }

    void notifyEvent(Player::ObservableEvent event);
    void resetRoundData(void);

    Player&   _player;
};

// More definition for events
//
struct EvAction       : sc::event < EvAction >
{
    EvAction(void) { TEMP_LOG("EvAction event"); }
};
struct EvFlip       : sc::event < EvFlip >
{
    EvFlip(void) { TEMP_LOG("EvFlip event"); }
};
struct EvWon     : sc::event < EvWon>
{
    EvWon(void) { TEMP_LOG("EvWinner event"); }
};
struct EvTie  : sc::event < EvTie >
{
    EvTie(void) { TEMP_LOG("EvWinnerTie event"); }
};
struct EvLost      : sc::event < EvLost >
{
    EvLost() { TEMP_LOG("EvLoser event"); }
};

struct EvAcceptCards : sc::event < EvAcceptCards>
{
    EvAcceptCards() { TEMP_LOG("EvWinnerReqCards event"); }
};

struct EvReset      : sc::event < EvReset >
{
    EvReset(void) { TEMP_LOG("EvReset event"); }
};

struct EvTimeout    : sc::event < EvTimeout >
{
    EvTimeout(void) { TEMP_LOG("EvTimeout event"); }
};

// More definition for states
//
struct Idle : sc::state<Idle, PlayerSM>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvAction > > reactions;

    Idle(my_context ctx);
    ~Idle(void);

    sc::result react(const EvAction& event);
};

struct Eliminated : sc::state<Eliminated, PlayerSM>
{
    Eliminated(my_context ctx);
    ~Eliminated(void);
};

struct AcceptingCards: sc::state<AcceptingCards, PlayerSM>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvAction>,
    sc::custom_reaction< EvTimeout>,
    sc::transition< EvAcceptCards, Idle> > reactions;

    AcceptingCards(my_context ctx);
    ~AcceptingCards(void);

    sc::result react(const EvAction& event);
    sc::result react(const EvTimeout& event);
};

struct CardsPlayed : sc::state<CardsPlayed, PlayerSM, WaitForWinner>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvLost >,
    sc::transition< EvWon, AcceptingCards> > reactions;

    CardsPlayed(my_context ctx);
    ~CardsPlayed(void);

    sc::result react(const EvLost& event);
};

struct WaitForWinner : sc::state<WaitForWinner, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvTie> > reactions;

    WaitForWinner(my_context ctx);
    ~WaitForWinner(void);

    sc::result react(const EvTie& event);
};

struct WaitHoleCard : sc::state<WaitHoleCard, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvAction > > reactions;

    WaitHoleCard(my_context ctx);
    ~WaitHoleCard(void);

    sc::result react(const EvAction& event);
};

struct WaitLastCard : sc::state<WaitLastCard, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvAction > > reactions;

    WaitLastCard(my_context ctx);
    ~WaitLastCard(void);

    sc::result react(const EvAction& event);
};

struct WaitFlip : sc::state<WaitFlip, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvAction > > reactions;

    WaitFlip(my_context ctx);
    ~WaitFlip(void);

    sc::result react(const EvAction& event);
};



