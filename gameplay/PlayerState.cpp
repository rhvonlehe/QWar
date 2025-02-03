#include "PlayerState.h"
#include "Player.h"

namespace gameplay {


Idle::Idle(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("Idle state entered");
    resetRoundData(player);
    notifyObservers(player, Player::EV_PLAYER_ACTIVE);
}

Idle::~Idle()
{}

sc::result Idle::react(const EvAction& event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name();
    TEMP_LOG(" rcvd EvAction in Idle state");
    playCard(player, Card::FACEUP);
    setEvalCard(player);

    return transit<WaitForWinner>();
}

Eliminated::Eliminated(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("Eliminated state entered");
    notifyObservers(player, Player::EV_PLAYER_ELIMINATED);
}

Eliminated::~Eliminated()
{}

AcceptingCards::AcceptingCards(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("AcceptingCards state entered");
    cancelTimer(player);
    startTimer(player, 500);
    notifyObservers(player, Player::EV_WINNER);
    notifyObservers(player, Player::EV_PLAYER_ACTIVE);
}

AcceptingCards::~AcceptingCards()
{}

sc::result AcceptingCards::react(const EvAction &event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name();
    TEMP_LOG(" rcvd EvAction in AcceptingCards state");
    notifyObservers(player, Player::EV_WINNER_REQ_CARDS);

    return discard_event();
}

sc::result AcceptingCards::react(const EvTimeout& event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("received TIMEOUT in state AcceptingCards");
    post_event(EvAction());

    return discard_event();
}

CardsPlayed::CardsPlayed(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("CardsPlayed state entered");
}

CardsPlayed::~CardsPlayed()
{
}

sc::result CardsPlayed::react(const EvLost& event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name();
    TEMP_LOG(" rcvd EvLost in CardsPlayed state");


    if (player.outOfCards())
    {
        return transit<Eliminated>();
    }

    return transit<Idle>();
}

WaitForWinner::WaitForWinner(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("WaitForWinner state entered");
    notifyObservers(player, Player::EV_PLAYER_WAITING);
}

WaitForWinner::~WaitForWinner(void)
{}

sc::result WaitForWinner::react(const EvTie& event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name();
    TEMP_LOG(" rcvd evTie in WaitForWinner state");

    if (player.outOfCards())
    {
        return transit<Eliminated>();
    }

    return transit<WaitHoleCard>();
}

WaitHoleCard::WaitHoleCard(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("WaitHoleCard state entered");

    notifyObservers(player, Player::EV_PLAYER_ACTIVE);
}

WaitHoleCard::~WaitHoleCard(void)
{}

sc::result WaitHoleCard::react(const EvAction& event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name();
    TEMP_LOG(" rcvd EvAction in WaitHoleCard state");
    playCard(player, Card::FACEDOWN);
    setEvalCard(player);

    if (player.outOfCards())
    {
        return transit<WaitFlip>();
    }

    return transit<WaitLastCard>();
}


WaitLastCard::WaitLastCard(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("WaitLastCard state entered");

    notifyObservers(player, Player::EV_PLAYER_ACTIVE);
}

WaitLastCard::~WaitLastCard(void)
{}

sc::result WaitLastCard::react(const EvAction& event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name();
    TEMP_LOG(" rcvd EvAction in WaitLastCard state");
    playCard(player, Card::FACEUP);
    return transit<WaitFlip>();
}

WaitFlip::WaitFlip(my_context ctx)
    : my_base(ctx)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name() << " ";
    TEMP_LOG("WaitFlip state entered");

    notifyObservers(player, Player::EV_PLAYER_ACTIVE);
}

WaitFlip::~WaitFlip(void)
{}

sc::result WaitFlip::react(const EvAction& event)
{
    auto& player = context<PlayerSM>().player_;
    std::cout << player.name();
    TEMP_LOG(" rcvd EvAction in WaitFlip state");
    flipCard(player);
    notifyObservers(player, Player::EV_CARD_FLIPPED);
    return transit<WaitForWinner>();
}

} // gameplay
