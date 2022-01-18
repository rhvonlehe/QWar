#include "RoundState.h"
#include <Round.h>

#include <cstdio> // temp todo remove



RoundSM::RoundSM(my_context ctx, Round& round)
    : my_base(ctx),
    _round(round)

{
    assert(_round._players.size());
}

void RoundSM::handlePlayerWaiting(std::shared_ptr<Player> player)
{
    assert(_round._players.size());

    _round.handlePlayerWaiting(player);
}

void RoundSM::initializeRound(void)
{
    assert(_round._players.size());

    _round.initializeRound();
}

void RoundSM::distributeCards(std::shared_ptr<Player> player)
{
    assert(_round._players.size());

    _round.distributeCards(player);
    _round.cullPlayerList();
}

EvWinner::EvWinner(std::shared_ptr<Player> player)
    : player(player)
{
    TEMP_LOG("EvWinner event");
}

Active::Active(my_context ctx)
    : my_base(ctx)
{
    context<RoundSM>().initializeRound();
    TEMP_LOG("Normal state entered");
}

Active::~Active(void)
{}

sc::result Active::react( const EvPlayerWaiting& event )
{
    context<RoundSM>().handlePlayerWaiting(event.player);

    return discard_event();
}

sc::result Active::react(const EvWinner& event)
{
    event.player->won();

    return transit<Done>();
}

Done::Done(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("Done state entered");
}

Done::~Done(void)
{}

sc::result Done::react(const EvDistributeCards& event)
{
    context<RoundSM>().distributeCards(event.player);

    return transit<Active>();
}
