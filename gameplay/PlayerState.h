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
struct WaitFirstCard;
struct WaitHoleCard;
struct WaitLastCard;
struct WaitFlip;

struct PlayerSM : sc::asynchronous_state_machine<PlayerSM, Idle>
{
    PlayerSM(my_context ctx, Player& player)
        : my_base(ctx),
          _player(player) {}
    ~PlayerSM(void) { terminate(); }

    void playOneCard(const EvAction& event);
    void notifyEvent(Player::ObservableEvent event);
    void resetRoundData(void);

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
    sc::transition< EvAction, WaitForWinner,
    PlayerSM, &PlayerSM::playOneCard > > reactions;

    Idle(my_context ctx);
    ~Idle(void);
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
    sc::transition< EvTie, WaitFirstCard > > reactions;

    WaitForWinner(my_context ctx);
    ~WaitForWinner(void);
};

struct WaitFirstCard : sc::state<WaitFirstCard, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::transition< EvAction, WaitHoleCard > > reactions;

    WaitFirstCard(my_context ctx);
    ~WaitFirstCard(void);
};

struct WaitHoleCard : sc::state<WaitHoleCard, CardsPlayed>
{

    WaitHoleCard(my_context ctx);
    ~WaitHoleCard(void);
};
//struct



