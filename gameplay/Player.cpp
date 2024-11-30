#include "Player.h"
#include <PlayerState.h>
#include <assert.h>
#include <thread>

Player::Player(const std::string name)
    : name_(name),
      scheduler_(true)
{
    // NOTE: It might be a better idea to have 2-stage initialization of Player
    // and start the thread during the 2nd stage outside of the construction.

    // Set up event processor
    processor_ = scheduler_.create_processor<PlayerSM>(std::ref(*this));
    scheduler_.initiate_processor(processor_);

    stateThread_ = std::thread( [&]() {
        std::cout << "starting player SM thread" << std::endl;
        scheduler_();
    } );

    ioCtxThread_ = std::thread( [&]() {
        work_ = std::make_unique<ba::executor_work_guard<ba::io_context::executor_type>>(ba::make_work_guard(io_));
        std::cout << "starting player asio thread" << std::endl;
        io_.run();
    });
}

Player::~Player(void)
{
    // Caution: one can't delete a deadline timer if the io object it is using
    // is gone.  It stores that internally as a reference.  Calling cancel here
    // makes sure that it won't try to use the _io it references when it is
    // destroyed at the end of this destructor.
    if (timer_) timer_->cancel();
    scheduler_.terminate();
    stateThread_.join();
    work_->reset();
    ioCtxThread_.join();
}

void Player::reset(void)
{
    unplayedPile_.clear();
    playedPile_.clear();

    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvReset>(new EvReset()));
}

void Player::action(void)
{
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvAction>(new EvAction()));
}

void Player::playCard(bool faceDown)
{
    auto card = getNextCard();

    card.flip(faceDown);
    activeRoundCards_.push_back(card);
    notifyEvent(EV_CARD_PLAYED);
    notifyEvent(EV_CARDS_CHANGED);
}

void Player::flipCard(void)
{
    assert(activeRoundCards_.size() > evalCard_);
    activeRoundCards_[evalCard_].flip(false);
    notifyEvent(EV_CARDS_CHANGED);
}


Card& Player::evalCard(void)
{
    assert(activeRoundCards_.size() > evalCard_);
    return activeRoundCards_[evalCard_];
}

void Player::tie(void)
{
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvTie>(new EvTie()));
}

void Player::won()
{
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvWon>(new EvWon()));
}

std::vector<Card> Player::lost(void)
{
    auto retVal = activeRoundCards_;
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvLost>(new EvLost()));
    return retVal;
}

void Player::addObserverCallback(const std::function<void (Player::ObservableEvent)> func)
{
    observerFuncs_.push_back(func);
}

Card Player::getNextCard()
{
    assert(!outOfCards());

    // Shuffle played deck and make it current deck if needed
    //
    if (unplayedPile_.isEmpty())
    {
        if (!playedPile_.isEmpty())
        {
            playedPile_.shuffle();
            movePlayedToCurrent();
        }
    }

    // This call decrements the pile
    auto nextCard = unplayedPile_.nextCard();

    return nextCard;
}

void Player::setEvalCard(void)
{
    assert(activeRoundCards_.size() > 0);
    evalCard_ = activeRoundCards_.size() - 1;
}

void Player::resetRoundData(void)
{
    TEMP_LOG("Resetting round data");
    activeRoundCards_.clear();
    evalCard_ = 0;
}

void Player::acceptRoundCards(const Pile pile, const std::vector<Card> cards)
{
    fflush(stdout);
    Deck& deck = (PILE_UNPLAYED == pile) ? unplayedPile_ : playedPile_;

    std::cout << "player " << name_ << " accepting " << cards.size() << " loser cards" << std::endl;
    deck.addBack(cards);
    std::cout << "player " << name_ << " accepting " << activeRoundCards_.size() << " own cards" << std::endl;
    deck.addBack(activeRoundCards_);

    notifyEvent(EV_CARDS_CHANGED);

    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvAcceptCards>(new EvAcceptCards()));
}

void Player::acceptDealtCard(const Pile pile, const Card card)
{
    PILE_UNPLAYED == pile ? unplayedPile_.addBack(card) :
                            playedPile_.addBack(card);

    notifyEvent(EV_CARDS_CHANGED);
}

void Player::movePlayedToCurrent()
{
    while (false == playedPile_.isEmpty())
    {
        unplayedPile_.addBack(playedPile_.nextCard());
    }
}

void Player::notifyEvent(ObservableEvent event)
{
    std::for_each(observerFuncs_.begin(), observerFuncs_.end(),
                  [event](std::function<void(ObservableEvent)> f) { f(event); });
}

void Player::startTimer(const boost::posix_time::milliseconds ms)
{
    timer_ = std::make_unique<ba::deadline_timer>(io_, ms);
    timer_->async_wait([&](const boost::system::error_code&) {
        scheduler_.queue_event(processor_,
                               boost::intrusive_ptr<EvTimeout>(new EvTimeout()));
    });
}

void Player::cancelTimer(void)
{
    timer_->cancel();
}



