#include "Player.h"
#include <assert.h>
#include <thread>

Player::Player(const std::string name)
    : _name(name),
      _scheduler(true)
{
    // Set up event processor
    _processor = _scheduler.create_processor<PlayerSM>(this);
    _scheduler.initiate_processor(_processor);

    _processorThread =
            std::make_unique<std::thread>(
                [&]()
    {
        std::cout << "starting player SM thread" << std::endl;
        _scheduler();
    }
    );
}

Player::~Player(void)
{
    _scheduler.terminate();
    _processorThread->join();
}

void Player::reset(void)
{
    _unplayedPile.clear();
    _playedPile.clear();

    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvReset>(new EvReset()));
}

// pre-condition: at least one card available in Player's piles
//
void Player::playCard(void)
{
    _activeRoundCards.push_back(getNextCard());
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvPlay>(new EvPlay()));
    notifyEvent(EV_CARD_PLAYED);
    notifyEvent(EV_CARDS_CHANGED);
}

void Player::addObserverCallback(const std::function<void (Player::ObservableEvent)> func)
{
    _observerFuncs.push_back(func);
}

std::shared_ptr<Card> Player::getNextCard()
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
            _scheduler.queue_event(_processor,
                                   boost::intrusive_ptr<EvOutOfCards>(new EvOutOfCards()));
            return nullptr;
        }
    }

    return _unplayedPile.nextCard();
}

void Player::acceptNewCards(const Pile pile, const std::vector<std::shared_ptr<Card>> cards)
{
    Deck& deck = (UNPLAYED == pile) ? _unplayedPile : _playedPile;

    deck.addBack(cards);

    notifyEvent(EV_CARDS_CHANGED);
}

void Player::acceptNewCard(const Pile pile, const std::shared_ptr<Card> card)
{
    UNPLAYED == pile ? _unplayedPile.addBack(card) :
                      _playedPile.addBack(card);

    notifyEvent(EV_CARDS_CHANGED);
}

void Player::movePlayedToCurrent()
{
    while (false == _playedPile.isEmpty())
    {
        _unplayedPile.addBack(_playedPile.nextCard());
    }
}

void Player::notifyEvent(ObservableEvent event)
{
    std::for_each(_observerFuncs.begin(), _observerFuncs.end(),
                  [event](std::function<void(ObservableEvent)> f) { f(event); });
}
