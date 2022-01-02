#pragma once

#include <boost/statechart/event.hpp>
#include <boost/statechart/asynchronous_state_machine.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>

#include <iostream>
#define TEMP_LOG(X) { std::cout << X << std::endl; }

namespace sc = boost::statechart;

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

struct PlayerSM : sc::asynchronous_state_machine<PlayerSM, Idle>
{
    PlayerSM(my_context ctx, Player* player)
        : my_base(ctx),
          _player(player) {}
    ~PlayerSM(void) { terminate(); }

    void playOneCard(const EvPlay& event);

private:
    Player*   _player;
};

// More definition for events
//
struct EvOutOfCards : sc::event < EvOutOfCards >
{
    EvOutOfCards(void) { TEMP_LOG("EvOutOfCards event"); }
};
struct EvPlay       : sc::event < EvPlay >
{
    EvPlay(void) { TEMP_LOG("EvPlay event"); }
};
struct EvFlip       : sc::event < EvFlip >
{
    EvFlip(void) { TEMP_LOG("EvFlip event"); }
};
struct EvWinner     : sc::event < EvWinner>
{
    EvWinner(void) { TEMP_LOG("EvWinner event"); }
};
struct EvWinnerTie  : sc::event < EvWinnerTie >
{
    EvWinnerTie(void) { TEMP_LOG("EvWinnerTie event"); }
};
struct EvLoser      : sc::event < EvLoser >
{
    EvLoser(void) { TEMP_LOG("EvLoser event"); }
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
    sc::transition< EvPlay, WaitForWinner,
    PlayerSM, &PlayerSM::playOneCard > > reactions;

    Idle(my_context ctx);
    ~Idle(void);
};

struct Eliminated : sc::state<Eliminated, PlayerSM>
{
    Eliminated(my_context ctx);
    ~Eliminated(void);
};

struct CardsPlayed : sc::state<CardsPlayed, PlayerSM, WaitForWinner>
{
    typedef boost::mpl::list<
    sc::transition< EvOutOfCards, Eliminated >,
    sc::transition< EvLoser, Idle>,
    sc::transition< EvWinner, Idle> > reactions;

    CardsPlayed(my_context ctx);
    ~CardsPlayed(void);
};

struct WaitForWinner : sc::state<WaitForWinner, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::transition< EvWinnerTie, WaitFirstCard > > reactions;

    WaitForWinner(my_context ctx);
    ~WaitForWinner(void);
};

struct WaitFirstCard : sc::state<WaitFirstCard, CardsPlayed>
{
    typedef boost::mpl::list<
    sc::transition< EvPlay, WaitHoleCard > > reactions;

    WaitFirstCard(my_context ctx);
    ~WaitFirstCard(void);
};

struct WaitHoleCard : sc::state<WaitHoleCard, CardsPlayed>
{

    WaitHoleCard(my_context ctx);
    ~WaitHoleCard(void);
};
//struct



