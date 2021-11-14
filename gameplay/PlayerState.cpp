#include "PlayerState.h"
#include "Player.h"

void PlayerState::changeState(Player& player, PlayerState* next)
{
    player.changeState(next);
}

void PlayerState::outOfCards(Player& player)
{
    player.changeState(Eliminated::instance());
}

PlayerState* Eliminated::instance(void)
{
    static Eliminated state;
    return &state;
}

PlayerState* Idle::instance(void)
{
    static Idle state;
    return &state;
}

void Idle::playFirst(Player& player)
{

}




