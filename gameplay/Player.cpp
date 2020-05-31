#include "Player.h"
#include <assert.h>

Player::Player(const std::string name)
    : _name(name)
{

}

// pre-condition: at least one card available in Player's piles
//
Card Player::playCard()
{
    // Shuffle played deck and make it current deck if needed
    //
    if (_currentDeck.isEmpty())
    {
        if (!_playedDeck.isEmpty())
        {
            _playedDeck.shuffle();
            movePlayedToCurrent();
        }
        else
        {
            assert(1);
        }
    }

    Card card = _currentDeck.nextCard();

    return card;
}

void Player::acceptNewCards(const Pile pile, const std::vector<Card>& cards)
{
    Deck& deck = (CURRENT == pile) ? _currentDeck : _playedDeck;

    deck.addBack(cards);
}

void Player::acceptNewCard(const Pile pile, const Card card)
{
    CURRENT == pile ? _currentDeck.addBack(card) :
                      _playedDeck.addBack(card);
}

void Player::movePlayedToCurrent()
{
    while (false == _playedDeck.isEmpty())
    {
        _currentDeck.addBack(_playedDeck.nextCard());
    }
}
