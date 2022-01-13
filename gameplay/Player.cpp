#include "Player.h"
#include <PlayerState.h>
#include <assert.h>
#include <thread>

Player::Player(const std::string name)
    : _name(name),
      _scheduler(true)
{
    // Set up event processor
    _processor = _scheduler.create_processor<PlayerSM>(std::ref(*this));
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

void Player::action(void)
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvAction>(new EvAction()));
}

void Player::playCard(bool faceDown)
{
    auto card = getNextCard();

    if (nullptr != card)
    {
        card->flip(faceDown);
        _activeRoundCards.push_back(card);
        notifyEvent(EV_CARD_PLAYED);
        notifyEvent(EV_CARDS_CHANGED);
    }
}

void Player::flipCard(void)
{
    _evalCard->flip(false);
    notifyEvent(EV_CARDS_CHANGED);
}


std::shared_ptr<Card> Player::evalCard(void)
{
    return _evalCard;
}

void Player::tie(void)
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvTie>(new EvTie()));
}

void Player::won()
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvWon>(new EvWon()));
}

std::vector<std::shared_ptr<Card>> Player::lost(void)
{
    auto retVal = _activeRoundCards;
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvLost>(new EvLost()));
    return retVal;
}

void Player::addObserverCallback(const std::function<void (Player::ObservableEvent)> func)
{
    _observerFuncs.push_back(func);
}

std::shared_ptr<Card> Player::getNextCard()
{
    if (outOfCards())
    {
        _scheduler.queue_event(_processor,
                               boost::intrusive_ptr<EvOutOfCards>(new EvOutOfCards()));
        return nullptr;
    }
    // Shuffle played deck and make it current deck if needed
    //
    if (_unplayedPile.isEmpty())
    {
        if (!_playedPile.isEmpty())
        {
            _playedPile.shuffle();
            movePlayedToCurrent();
        }
    }

    return _unplayedPile.nextCard();
}

void Player::setEvalCard(void)
{
    assert(_activeRoundCards.size());
    _evalCard = _activeRoundCards.back();
}

void Player::resetRoundData(void)
{
    _evalCard = nullptr;
    _activeRoundCards.clear();
    notifyEvent(EV_PLAYER_ACTIVE);
}

// TODO: consider passing the pile,cards in the event
void Player::acceptRoundCards(const Pile pile, const std::vector<std::shared_ptr<Card>> cards)
{
    fflush(stdout);
    Deck& deck = (PILE_UNPLAYED == pile) ? _unplayedPile : _playedPile;

    std::cout << "player " << _name << " accepting " << cards.size() << " loser cards" << std::endl;
    deck.addBack(cards);
    std::cout << "player " << _name << " accepting " << _activeRoundCards.size() << " own cards" << std::endl;
    deck.addBack(_activeRoundCards);

    notifyEvent(EV_CARDS_CHANGED);

    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvAcceptCards>(new EvAcceptCards()));
}

void Player::acceptDealtCard(const Pile pile, const std::shared_ptr<Card> card)
{
    PILE_UNPLAYED == pile ? _unplayedPile.addBack(card) :
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
