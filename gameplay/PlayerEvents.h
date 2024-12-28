#pragma once

#include <boost/statechart/event.hpp>
#include <iostream>

#define TEMP_LOG(X) { std::cout << X << std::endl; }

namespace sc = boost::statechart;

namespace gameplay {

// definition of events
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


} // gameplay
