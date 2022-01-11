#include "PlayerState.h"
#include "Player.h"

void PlayerSM::playOneCard(const EvAction& event)
{
    _player.playCard();
    _player.setEvalCard();
    notifyEvent(Player::EV_PLAYER_WAIT_WINNER);
}

void PlayerSM::notifyEvent(Player::ObservableEvent event)
{
    _player.notifyEvent(event);
}

void PlayerSM::resetRoundData(void)
{
    _player.resetRoundData();
}

Idle::Idle(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("Idle state entered");
    context<PlayerSM>().resetRoundData();
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

AcceptingCards::AcceptingCards(my_context ctx)
    : my_base(ctx)
{
    context<PlayerSM>().notifyEvent(Player::EV_WINNER);
    context<PlayerSM>().notifyEvent(Player::EV_PLAYER_ACTIVE);
    TEMP_LOG("AcceptingCards state entered");
}

AcceptingCards::~AcceptingCards()
{}

sc::result AcceptingCards::react(const EvAction &event)
{
    context<PlayerSM>().
    context<PlayerSM>().notifyEvent(Player::EV_WINNER_REQ_CARDS);

    return discard_event();
}

CardsPlayed::CardsPlayed(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("CardsPlayed state entered");
}

CardsPlayed::~CardsPlayed()
{
}

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
