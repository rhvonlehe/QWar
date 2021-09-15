#include "Game.h"
#include <Card.h>
#include <Round.h>
#include <map>
#include <functional>
#include <iostream>
#include <array>

const std::array<Card::Suit, 4> cardSuits
{
    Card::SPADES, Card::CLUBS, Card::DIAMONDS, Card::HEARTS
};

const std::array<Card::Value, 13> cardValues
{
    Card::TWO, Card::THREE, Card::FOUR, Card::FIVE, Card::SIX, Card::SEVEN,
            Card::EIGHT, Card::NINE, Card::TEN, Card::JACK, Card::QUEEN,
            Card::KING, Card::ACE
};


Game::Game(std::vector<std::shared_ptr<Player>>& players)
    : _activePlayers(players),
      _roundNumber(0)
{
    initDeck();

    deal();
}

// For test purposes - todo update with latest changes that better support view integration
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

void Game::initDeck(void)
{
    // This is the only place a Card is created on the heap.  Everywhere
    // else it's using an already-created card via s-ptr.
    for (auto value : cardValues)
    {
        for (auto suit : cardSuits)
        {
            _deck.addBack(std::make_shared<Card>(suit, value));
        }
    }
}

void Game::deal()
{
    _deck.print();
    _deck.shuffle();
    _deck.print();

    while (!_deck.isEmpty())
    {
        for (auto& player : _activePlayers)
        {
            player->acceptNewCard(Player::CURRENT, _deck.nextCard());
            if (_deck.isEmpty())
            {
                break;
            }
        }
    }
}

void Game::cullPlayerList()
{
    _activePlayers.erase(std::remove_if(
                             _activePlayers.begin(), _activePlayers.end(),
                             [](const std::shared_ptr<Player>& x)
    {
        return x->outOfCards();
    }), _activePlayers.end());

}



