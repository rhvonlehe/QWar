#include "Game.h"
#include <Card.h>
#include <Round.h>
#include <EventScheduler.h>
#include <map>
#include <functional>
#include <iostream>

namespace gameplay {

#if 0
Game::Game(std::vector<std::unique_ptr<Player> >& players)
    : _allPlayers(std::move(players)),
      _roundNumber(0)
{
    // Create a vector of pointers to player to give to the round
    std::vector<Player*> roundPlayers;
    for (auto& player : _allPlayers)
    {
        roundPlayers.push_back(player.get());
        _activePlayers.push_back(player.get());
    }

    _round = std::make_unique<Round>(roundPlayers, [&]() { handleRoundComplete(); }); // TODO make use of this

    for (auto& player : _allPlayers)
    {
        player->addObserverCallback([&](Player::ObservableEvent event) {
            handlePlayerUpdate(player.get(), event);
        } );
    }
}
#endif

Game::Game(std::vector<std::string>& playerNames)
{

    std::vector<Player*> roundPlayers;

    for_each(playerNames.begin(), playerNames.end(),
             [&](std::string name) {
        allPlayers_[name] = std::make_unique<Player>(name, eventScheduler_);
        Player* player = allPlayers_[name].get();
        activePlayers_.push_back(player);
        roundPlayers.push_back(player);

        player->addObserverCallback([&, player](Player::ObservableEvent event) {
            handlePlayerUpdate(player, event);
        } );
    });

    round_ = std::make_unique<Round>(roundPlayers, [&]() { handleRoundComplete(); }); // TODO make use of this
}

Player& Game::getPlayer(const std::string name)
{
    return *allPlayers_[name];
}


#if 0 // todo
// For test purposes - todo update with latest changes that better support view support
void Game::autoPlay()
{
}
#endif

void Game::deal()
{
    deck_ = std::make_unique<Deck>(true);

    deck_->print();
    deck_->shuffle();
    deck_->print();

    // Also make sure no player is holding any cards
    std::for_each(activePlayers_.begin(), activePlayers_.end(),
                  [](Player* player) { player->reset(); } );

    while (!deck_->isEmpty())
    {
        for (auto& player : activePlayers_)
        {
            player->acceptDealtCard(Player::PILE_UNPLAYED, deck_->nextCard());
            if (deck_->isEmpty())
            {
                break;
            }
        }
    }
}

// todo - do something?
void Game::handleRoundComplete(void)
{
}

void Game::handlePlayerUpdate(Player* player,
                              Player::ObservableEvent event)
{
    switch (event)
    {
    case Player::EV_PLAYER_WAITING:
        round_->playerWaiting(player);
        break;
    case Player::EV_PLAYER_ELIMINATED:
        round_->playerEliminated(player);
        break;
    case Player::EV_WINNER_REQ_CARDS:
        round_->winnerReqCards(player);
        break;        
    default: break;
    }
}

} // gameplay




