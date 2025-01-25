#pragma once

// This player observes the events from the actual player in the overall model and
// will act like a real player.
#include <Player.h>

namespace gameplay {

class TestPlayer
{
public:
    TestPlayer(void) = delete;
    TestPlayer(Player * const player);

    void onPlayerEvent(Player* player, Player::ObservableEvent event);
    void doAction(void) { player_->action(); }

private:

    Player* const player_;
};

} // gameplay
