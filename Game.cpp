#include "Game.h"
#include <Card.h>
#include <Round.h>
#include <QDebug>
#include <map>
#include <functional>
#include <iostream>

const std::vector<Card::Suit> cardSuits
{
    Card::SPADES, Card::CLUBS, Card::DIAMONDS, Card::HEARTS
};

const std::vector<Card::Value> cardValues
{
    Card::TWO, Card::THREE, Card::FOUR, Card::FIVE, Card::SIX, Card::SEVEN,
            Card::EIGHT, Card::NINE, Card::TEN, Card::JACK, Card::QUEEN,
            Card::KING, Card::ACE
};



Game::Game(std::vector<Player>& players)
    : _activePlayers(players),
      _roundNumber(0)
{
    initDeck();
    _deck.print();
    _deck.shuffle();
    _deck.print();

    deal();
}

void Game::play()
{
    while (!isOver())
    {
        _roundNumber++;

        Round round(_activePlayers);
        round.play();

        cullPlayerList();
    }
}

void Game::initDeck(void)
{
    for (auto value : cardValues)
    {
        for (auto suit : cardSuits)
        {
            _deck.addBack(Card(suit, value));
        }
    }
}

void Game::deal()
{
    while (!_deck.isEmpty())
    {
        for (auto& player : _activePlayers)
        {
            player.acceptNewCard(Player::CURRENT,_deck.nextCard());
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
                             [](const Player& x)
    {
        return x.outOfCards();
    }), _activePlayers.end());

    if (_activePlayers.size() == 1)
    {
        std::cout << "Player " << _activePlayers[0].name() << " has won!" << std::endl;
    }
}



