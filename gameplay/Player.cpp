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
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvPlay>(new EvPlay()));
}

void Player::registerCallback(const std::function<void (Player::ObservableEvent)> callback)
{
    _callback = callback;
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

    _callback(EV_CARDS_CHANGED);
}

void Player::movePlayedToCurrent()
{
    while (false == _playedPile.isEmpty())
    {
        _unplayedPile.addBack(_playedPile.nextCard());
    }
}
