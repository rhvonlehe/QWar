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
struct EvOutOfCards;
struct EvAction;
struct EvFlip;
struct EvWon;
struct EvTie;
struct EvLost;
struct EvAction;
struct EvAcceptCards;
struct EvReset;

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
//    void handleFirstCard(const EvAction& event); // todo remove

    Player&   _player;
};

// More definition for events
//
struct EvOutOfCards : sc::event < EvOutOfCards >
{
    EvOutOfCards(void) { TEMP_LOG("EvOutOfCards event"); }
};
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
    sc::transition< EvAcceptCards, Idle> > reactions;

    AcceptingCards(my_context ctx);
    ~AcceptingCards(void);

    sc::result react(const EvAction& event);
};

struct CardsPlayed : sc::state<CardsPlayed, PlayerSM, WaitForWinner>
{
    typedef boost::mpl::list<
    sc::transition< EvOutOfCards, Eliminated >,
    sc::transition< EvLost, Idle>,
    sc::transition< EvWon, AcceptingCards> > reactions;

    CardsPlayed(my_context ctx);
    ~CardsPlayed(void);
};

struct WaitForWinner : sc::state<WaitForWinner, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::transition< EvTie, WaitHoleCard> > reactions;

    WaitForWinner(my_context ctx);
    ~WaitForWinner(void);
};

struct WaitHoleCard : sc::state<WaitHoleCard, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::custom_reaction< EvAction >
    /*sc::transition< EvAction, WaitHoleCard >*/ > reactions;

    WaitHoleCard(my_context ctx);
    ~WaitHoleCard(void);

    sc::result react(const EvAction& event);
};

struct WaitLastCard : sc::state<WaitLastCard, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::transition< EvOutOfCards, WaitFlip >,
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



