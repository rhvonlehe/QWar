#include "PlayerState.h"
#include "Player.h"

void PlayerSM::playOneCard(const EvPlay& event)
{
    _player->setEvalCard();
    _player->notifyEvent(Player::EV_PLAYER_WAITING);
}

Idle::Idle(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("Idle state entered");
}

Idle::~Idle()
{}

Eliminated::Eliminated(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("Eliminated state entered");
}

Eliminated::~Eliminated()
{}

CardsPlayed::CardsPlayed(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("CardsPlayed state entered");
}

CardsPlayed::~CardsPlayed()
{}

WaitForWinner::WaitForWinner(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("WaitForWinner state entered");
}

WaitForWinner::~WaitForWinner(void)
{}

WaitFirstCard::WaitFirstCard(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("WaitFirstCard state entered");
}

WaitFirstCard::~WaitFirstCard(void)
{}

WaitHoleCard::WaitHoleCard(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("WaitHoleCard state entered");
}

WaitHoleCard::~WaitHoleCard(void)
{}
