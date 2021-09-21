#include "Player.h"
#include <assert.h>

Player::Player(const std::string name)
    : _name(name)
{
}


// pre-condition: at least one card available in Player's piles
//
std::shared_ptr<Card> Player::playCard()
{
    // Shuffle played deck and make it current deck if needed
    //
    if (_unplayedCards.isEmpty())
    {
        if (!_playedCards.isEmpty())
        {
            _playedCards.shuffle();
            movePlayedToCurrent();
        }
        else
        {
            assert(1);
        }
    }



    auto card = _unplayedCards.nextCard();

    return card;
}

void Player::acceptNewCards(const Pile pile, const std::vector<std::shared_ptr<Card>> cards)
{
    Deck& deck = (CURRENT == pile) ? _unplayedCards : _playedCards;

    deck.addBack(cards);
}

void Player::acceptNewCard(const Pile pile, const std::shared_ptr<Card> card)
{
    CURRENT == pile ? _unplayedCards.addBack(card) :
                      _playedCards.addBack(card);
}

void Player::movePlayedToCurrent()
{
    while (false == _playedCards.isEmpty())
    {
        _unplayedCards.addBack(_playedCards.nextCard());
    }
}
