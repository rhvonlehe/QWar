#include "TestPlayer.h"
#include <iostream>

using namespace std;

namespace gameplay {

TestPlayer::TestPlayer(Player* const player) :
    player_(player)
{
    player_->addObserverCallback([&, player](Player::ObservableEvent event) {
        onPlayerEvent(player, event);
    });

}

void TestPlayer::onPlayerEvent(Player* player, Player::ObservableEvent event)
{
    auto name = player->name();
    cout << player->name() << " TestPlayer::onPlayerEvent event type " << event << endl;

    if (Player::EV_PLAYER_ACTIVE == event)
    {
        player->action();
    }
}

} // gameplay
