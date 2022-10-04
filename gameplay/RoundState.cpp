#include "RoundState.h"
#include <Round.h>


RoundSM::RoundSM(my_context ctx, Round& round)
    : my_base(ctx),
    _round(round)

{
    assert(_round._players.size());
}

EvWinner::EvWinner(Player *player)
    : player(player)
{
    TEMP_LOG("EvWinner event");
}

Active::Active(my_context ctx)
    : my_base(ctx)
{
    context<RoundSM>()._round.initializeRound();
    TEMP_LOG("Normal state entered");
}

Active::~Active(void)
{}

sc::result Active::react( const EvPlayerWaiting& event )
{
    auto& round = context<RoundSM>()._round;

    round.handlePlayerWaiting(event.player);

    return discard_event();
}

sc::result Active::react(const EvPlayerEliminated& event)
{
    auto& round = context<RoundSM>()._round;

    round.handlePlayerEliminated(event.player);
    round.evaluate();  // Need this in the Active state in case he's the last player left

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
    context<RoundSM>()._round.distributeCards(event.player);

    return transit<Active>();
}

sc::result Done::react(const EvPlayerEliminated& event)
{
    auto& round = context<RoundSM>()._round;

    round.handlePlayerEliminated(event.player);

    return discard_event();
}


