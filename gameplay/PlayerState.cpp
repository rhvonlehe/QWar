#include "PlayerState.h"

Idle::Idle(my_context ctx)
    : my_base(ctx)
{}

Idle::~Idle()
{}

Eliminated::Eliminated(my_context ctx)
    : my_base(ctx)
{}

Eliminated::~Eliminated()
{}

CardsPlayed::CardsPlayed(my_context ctx)
    : my_base(ctx)
{}

CardsPlayed::~CardsPlayed()
{}
