#include "Player.h"
#include <assert.h>

Player::Player(const std::string name)
    : _name(name)
{
//    _playerState = Idle::instance();
}


// pre-condition: at least one card available in Player's piles
//
void Player::playCard(void)
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvPlay>(new EvPlay()));
}

#if 0 // todo old remove
std::shared_ptr<Card> Player::playCard()
{
    // Shuffle played deck and make it current deck if needed
    //
    if (_unplayedPile.isEmpty())
    {
        if (!_playedPile.isEmpty())
        {
            _playedPile.shuffle();
            movePlayedToCurrent();
        }
        else
        {
            assert(1);
        }
    }



    auto card = _unplayedPile.nextCard();

    return card;
}
#endif

void Player::acceptNewCards(const Pile pile, const std::vector<std::shared_ptr<Card>> cards)
{
    Deck& deck = (CURRENT == pile) ? _unplayedPile : _playedPile;

    deck.addBack(cards);
}

void Player::acceptNewCard(const Pile pile, const std::shared_ptr<Card> card)
{
    CURRENT == pile ? _unplayedPile.addBack(card) :
                      _playedPile.addBack(card);
}

void Player::movePlayedToCurrent()
{
    while (false == _playedPile.isEmpty())
    {
        _unplayedPile.addBack(_playedPile.nextCard());
    }
}

// TODO remove
//void Player::changeState(PlayerState* next)
//{
//    _playerState = next;
//}
