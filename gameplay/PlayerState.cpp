#include "PlayerState.h"
#include "Player.h"

void PlayerSM::notifyEvent(Player::ObservableEvent event)
{
    _player.notifyEvent(event);
}

void PlayerSM::resetRoundData(void)
{
    _player.resetRoundData();
}

// todo remove?
//void PlayerSM::handleFirstCard(const EvAction& event)
//{
//    _player.
//}


Idle::Idle(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("Idle state entered");
    context<PlayerSM>().resetRoundData();
}

Idle::~Idle()
{}

sc::result Idle::react(const EvAction& event)
{
    auto& player = context<PlayerSM>()._player;
    player.playCard();
    player.setEvalCard();

    return transit<WaitForWinner>();
}


Eliminated::Eliminated(my_context ctx)
    : my_base(ctx)
{
    context<PlayerSM>().notifyEvent(Player::EV_PLAYER_WAITING);
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
    context<PlayerSM>().notifyEvent(Player::EV_WINNER_REQ_CARDS);

    return discard_event();
}

sc::result AcceptingCards::react(const EvTimeout& event)
{
    post_event(EvAction());

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
    auto& player = context<PlayerSM>()._player;
    player.notifyEvent(Player::EV_PLAYER_WAITING);
    TEMP_LOG("WaitForWinner state entered");
}

WaitForWinner::~WaitForWinner(void)
{}

WaitHoleCard::WaitHoleCard(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("WaitHoleCard state entered");
    context<PlayerSM>().notifyEvent(Player::EV_PLAYER_ACTIVE);
}

WaitHoleCard::~WaitHoleCard(void)
{}

sc::result WaitHoleCard::react(const EvAction& event)
{
    auto& player = context<PlayerSM>()._player;
    player.playCard(true);
    player.setEvalCard();
    return transit<WaitLastCard>();
}


WaitLastCard::WaitLastCard(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("WaitLastCard state entered");
}

WaitLastCard::~WaitLastCard(void)
{}

sc::result WaitLastCard::react(const EvAction& event)
{
    auto& player = context<PlayerSM>()._player;
    auto cardFound = player.playCard();
    if (cardFound)
    {
        return transit<WaitFlip>();
    }
    return discard_event();
}

WaitFlip::WaitFlip(my_context ctx)
    : my_base(ctx)
{
    TEMP_LOG("WaitFlip state entered");
}

WaitFlip::~WaitFlip(void)
{}

sc::result WaitFlip::react(const EvAction& event)
{
    auto& player = context<PlayerSM>()._player;
    player.flipCard();
    player.notifyEvent(Player::EV_CARD_FLIPPED);
    return transit<WaitForWinner>();
}
