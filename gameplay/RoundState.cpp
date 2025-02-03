#include "RoundState.h"
#include <Round.h>

namespace gameplay {

RoundSM::RoundSM(my_context ctx, Round& round)
    : my_base(ctx),
    round_(round)

{
    assert(round_.players_.size());
}

EvWinner::EvWinner(Player *player)
    : player(player)
{
    TEMP_LOG("EvWinner event");
}

Active::Active(my_context ctx)
    : my_base(ctx)
{
    context<RoundSM>().round_.initializeRound();
    TEMP_LOG("Active state entered");
}

Active::~Active(void)
{}

sc::result Active::react( const EvPlayerWaiting& event )
{
    TEMP_LOG("rcvd EvPlayerWaiting in Active state");
    auto& round = context<RoundSM>().round_;

    round.handlePlayerWaiting(event.player);

    return discard_event();
}

sc::result Active::react(const EvPlayerEliminated& event)
{
    TEMP_LOG("rcvd EvPlayerEliminated in Active state");
    auto& round = context<RoundSM>().round_;

    round.handlePlayerEliminated(event.player);
// TODO remove    // round.evaluate();  // Need this in the Active state in case he's the last player left

    return discard_event();
}


sc::result Active::react(const EvWinner& event)
{
    TEMP_LOG("rcvd EvWinner in Active state");
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
    TEMP_LOG("rcvd EvDistributeCards in Done state");
    context<RoundSM>().round_.distributeCards(event.player);

    return transit<Active>();
}

sc::result Done::react(const EvPlayerEliminated& event)
{
    TEMP_LOG("rcvd EvPlayerEliminated in Done state");
    auto& round = context<RoundSM>().round_;

    round.handlePlayerEliminated(event.player);

    return discard_event();
}


} // gameplay
