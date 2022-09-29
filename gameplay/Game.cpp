#include "Game.h"
#include <Card.h>
#include <Round.h>
#include <map>
#include <functional>
#include <iostream>

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
        _allPlayers[name] = std::make_unique<Player>(name);
        Player* player = _allPlayers[name].get();
        _activePlayers.push_back(player);
        roundPlayers.push_back(player);

        player->addObserverCallback([&, player](Player::ObservableEvent event) {
            handlePlayerUpdate(player, event);
        } );
    });

    _round = std::make_unique<Round>(roundPlayers, [&]() { handleRoundComplete(); }); // TODO make use of this
}

Player& Game::getPlayer(const std::string name)
{
    return *_allPlayers[name];
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
                  [](Player* player) { player->reset(); } );

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

void Game::handlePlayerUpdate(Player* player,
                              Player::ObservableEvent event)
{
    switch (event)
    {
    case Player::EV_PLAYER_WAITING:
        _round->playerWaiting(player);
        break;
    case Player::EV_PLAYER_ELIMINATED:
        _round->playerEliminated(player);
        break;
    case Player::EV_WINNER_REQ_CARDS:
        _round->winnerReqCards(player);
        break;        
    default: break;
    }
}






