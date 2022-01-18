#include "Game.h"
#include <Card.h>
#include <Round.h>
#include <map>
#include <functional>
#include <iostream>

Game::Game(std::vector<std::shared_ptr<Player>>& players)
    : _activePlayers(players),
      _allPlayers(players),
      _roundNumber(0)
{
    _round = std::make_unique<Round>(_allPlayers, [&]() { handleRoundComplete(); }); // TODO make use of this

    for (auto& player : _allPlayers)
    {
        player->addObserverCallback([&](Player::ObservableEvent event) {
            handlePlayerUpdate(player, event);
        } );
    }
}

#if 0 // todo
// For test purposes - todo update with latest changes that better support view support
void Game::autoPlay()
{
    while (!isOver())
    {
        _roundNumber++;
        std::cout << "Round: " << _roundNumber << std::endl;

        Round round(_activePlayers);
        round.play();

        cullPlayerList();
    }

    if (_activePlayers.size() == 1)
    {
        std::cout << "Player " << _activePlayers[0]->name() << " has won!" << std::endl;
    }

}
#endif

void Game::deal()
{
    _deck = std::make_unique<Deck>(true);

    _deck->print();
    _deck->shuffle();
    _deck->print();

    // Also make sure no player is holding any cards
    std::for_each(_activePlayers.begin(), _activePlayers.end(),
                  [](std::shared_ptr<Player> player) { player->reset(); } );

    while (!_deck->isEmpty())
    {
        for (auto& player : _activePlayers)
        {
            player->acceptDealtCard(Player::PILE_UNPLAYED, _deck->nextCard());
            if (_deck->isEmpty())
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

void Game::handlePlayerUpdate(std::shared_ptr<Player> player,
                              Player::ObservableEvent event)
{
    switch (event)
    {
    case Player::EV_PLAYER_WAITING:
        _round->playerWaiting(player);
        break;
    case Player::EV_WINNER_REQ_CARDS:
        _round->winnerReqCards(player);
        break;
    default: break;
    }
}






