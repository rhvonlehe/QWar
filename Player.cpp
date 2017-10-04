#include "Player.h"

Player::Player(const std::string name)
    : _name(name)
{

}

Card Player::playCard()
{
    Card card = _currentDeck.nextCard();

    // Shuffle played deck and make it current deck if needed
    //
    if (_currentDeck.isEmpty())
    {
        _playedDeck.shuffle();
        movePlayedToCurrent();
    }

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
