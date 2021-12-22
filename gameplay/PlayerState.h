#pragma once

#include <boost/statechart/event.hpp>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>

#include <iostream>
#define TEMP_LOG(X) { std::cout << X << std::endl; }

// Declare things in one spot for visual convenience
//
class Player;

// Player events
//
struct EvOutOfCards;
struct EvPlay;
struct EvFlip;
struct EvWinner;
struct EvWinnterTie;
struct EvLoser;
struct EvReset;

// Player states
//
struct Idle;
struct Eliminated;
struct CardsPlayed;
struct WaitForWinner;
struct WaitFirstCard;
struct WaitHoleCard;
struct WaitLastCard;
struct WaitFlip;

struct PlayerSM : boost::statechart::asynchronous_state_machine<PlayerSM, Idle>
{
    PlayerSM(my_context ctx, Player* player)
        : my_base(ctx),
          _player(player) {}
    ~PlayerSM(void) { terminate(); }

private:
    Player*   _player;
};

// More definition for events
//
struct EvOutOfCards : boost::statechart::event < EvOutOfCards >
{
    EvOutOfCards(void) { TEMP_LOG("EvOutOfCards event"); }
};
struct EvPlay       : boost::statechart::event < EvPlay >
{
    EvPlay(void) { TEMP_LOG("EvPlay event"); }
};
struct EvFlip       : boost::statechart::event < EvFlip >
{
    EvFlip(void) { TEMP_LOG("EvFlip event"); }
};
struct EvWinner     : boost::statechart::event < EvWinner>
{
    EvWinner(void) { TEMP_LOG("EvWinner event"); }
};
struct EvWinnerTie  : boost::statechart::event < EvWinnerTie >
{
    EvWinnerTie(void) { TEMP_LOG("EvWinnerTie event"); }
};
struct EvLoser      : boost::statechart::event < EvLoser >
{
    EvLoser(void) { TEMP_LOG("EvLoser event"); }
};
struct EvReset      : boost::statechart::event < EvReset >
{
    EvReset(void) { TEMP_LOG("EvReset event"); }
};


// More definition for states
//
struct Idle : boost::statechart::state<Idle, PlayerSM>
{
    typedef boost::mpl::list<
    boost::statechart::transition< EvPlay, WaitForWinner > > reactions;

    Idle(my_context ctx);
    ~Idle(void);
};

struct Eliminated : boost::statechart::state<Eliminated, PlayerSM>
{
    Eliminated(my_context ctx);
    ~Eliminated(void);
};

struct CardsPlayed : boost::statechart::state<CardsPlayed, PlayerSM, WaitForWinner>
{
    typedef boost::mpl::list<
    boost::statechart::transition< EvOutOfCards, Eliminated >,
    boost::statechart::transition< EvLoser, Idle>,
    boost::statechart::transition< EvWinner, Idle> > reactions;

    CardsPlayed(my_context ctx);
    ~CardsPlayed(void);
};

struct WaitForWinner : boost::statechart::state<WaitForWinner, CardsPlayed>
{
    typedef boost::mpl::list<
    boost::statechart::transition< EvWinnerTie, WaitFirstCard > > reactions;

    WaitForWinner(my_context ctx);
    ~WaitForWinner(void);
};

struct WaitFirstCard : boost::statechart::state<WaitFirstCard, CardsPlayed>
{
    typedef boost::mpl::list<
    boost::statechart::transition< EvPlay, WaitHoleCard > > reactions;

    WaitFirstCard(my_context ctx);
    ~WaitFirstCard(void);
};

struct WaitHoleCard : boost::statechart::state<WaitHoleCard, CardsPlayed>
{

    WaitHoleCard(my_context ctx);
    ~WaitHoleCard(void);
};
//struct



