#include "Player.h"
#include <PlayerState.h>

// If I had really wanted to abstract away Boost::StateChart types here, I could have gone
// with an abstract factory for the creation of the event types. This would be huge overkill
// with likely no advantage.

namespace gameplay {

Player::Player(const std::string name, EventScheduler& scheduler)
    : name_(name),
      scheduler_(scheduler)
{
    scheduler_.createProcessor<PlayerSM>(std::ref(*this));

#if 0 // todo remove
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
#endif
}

Player::~Player(void)
{

#if 0 // todo remove
    // Caution: one can't delete a deadline timer if the io object it is using
    // is gone.  It stores that internally as a reference.  Calling cancel here
    // makes sure that it won't try to use the _io it references when it is
    // destroyed at the end of this destructor.
    if (timer_) timer_->cancel();
    scheduler_.terminate();
    stateThread_.join();
    work_->reset();
    ioCtxThread_.join();
#endif
}

void Player::reset(void)
{
    unplayedPile_.clear();
    playedPile_.clear();

    scheduler_.queueEvent<EvReset>(procHandle_);

#if 0 // todo remove
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvReset>(new EvReset()));
#endif
}

void Player::action(void)
{
    scheduler_.queueEvent<EvAction>(procHandle_);

#if 0 // todo remove
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvAction>(new EvAction()));
#endif
}

Card& Player::evalCard(void)
{
    assert(activeRoundCards_.size() > evalCard_);
    return activeRoundCards_[evalCard_];
}

void Player::tie(void)
{
    scheduler_.queueEvent<EvTie>(procHandle_);

#if 0 // todo remove
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvTie>(new EvTie()));
#endif
}

void Player::won()
{
    scheduler_.queueEvent<EvWon>(procHandle_);

#if 0 // todo remove
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvWon>(new EvWon()));
#endif
}

std::vector<Card> Player::lost(void)
{
    auto retVal = activeRoundCards_;

    scheduler_.queueEvent<EvLost>(procHandle_);

#if 0 //todo remove
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvLost>(new EvLost()));
#endif
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

void Player::acceptRoundCards(const Pile pile, const std::vector<Card> cards)
{
    fflush(stdout);
    Deck& deck = (PILE_UNPLAYED == pile) ? unplayedPile_ : playedPile_;

    std::cout << "player " << name_ << " accepting " << cards.size() << " loser cards" << std::endl;
    deck.addBack(cards);
    std::cout << "player " << name_ << " accepting " << activeRoundCards_.size() << " own cards" << std::endl;
    deck.addBack(activeRoundCards_);

    notifyObservers(*this, EV_CARDS_CHANGED);

    scheduler_.queueEvent<EvAcceptCards>(procHandle_);

#if 0 // todo remove
    scheduler_.queue_event(processor_,
                           boost::intrusive_ptr<EvAcceptCards>(new EvAcceptCards()));
#endif
}

void Player::acceptDealtCard(const Pile pile, const Card card)
{
    PILE_UNPLAYED == pile ? unplayedPile_.addBack(card) :
                            playedPile_.addBack(card);

    notifyObservers(*this, EV_CARDS_CHANGED);
}

void Player::movePlayedToCurrent()
{
    while (false == playedPile_.isEmpty())
    {
        unplayedPile_.addBack(playedPile_.nextCard());
    }
}

// ---------------------- Hidden Friends ----------------------------
//

void flipCard(Player& player)
{
    assert(player.activeRoundCards_.size() > player.evalCard_);
    player.activeRoundCards_[player.evalCard_].flip(Card::FACEUP);
    notifyObservers(player, Player::EV_CARDS_CHANGED);
}

void playCard(Player& player, Card::Face face)
{
    auto card = player.getNextCard();

    card.flip(face);
    player.activeRoundCards_.push_back(card);
    notifyObservers(player, Player::EV_CARD_PLAYED);
    notifyObservers(player, Player::EV_CARDS_CHANGED);
}

void startTimer(Player& player, const uint32_t ms)
{
    player.timerHandle_ = player.scheduler_.startTimer(player.procHandle_, ms);

#if 0
    player.timer_ = std::make_unique<ba::deadline_timer>(player.io_, ms);
    player.timer_->async_wait([&](const boost::system::error_code&) {
        player.scheduler_.queue_event(player.processor_,
                               boost::intrusive_ptr<EvTimeout>(new EvTimeout()));
    });
#endif
}

void cancelTimer(Player& player)
{
    player.scheduler_.stopTimer(player.timerHandle_);
}

void setEvalCard(Player& player)
{
    assert(player.activeRoundCards_.size() > 0);
    player.evalCard_ = player.activeRoundCards_.size() - 1;
}

void notifyObservers(Player& player, Player::ObservableEvent event)
{
    std::for_each(player.observerFuncs_.begin(), player.observerFuncs_.end(),
                  [event](std::function<void(Player::ObservableEvent)> f) { f(event); });
}

void resetRoundData(Player& player)
{
    TEMP_LOG("Resetting round data");
    player.activeRoundCards_.clear();
    player.evalCard_ = 0;
}



} // gameplay
