#include "Player.h"
#include <PlayerState.h>
#include <PlayerEvents.h>
#include <EventScheduler.h>

// If I had really wanted to abstract away Boost::StateChart types here, I could have gone
// with an abstract factory for the creation of the event types. This would be huge overkill
// with likely no advantage.

namespace gameplay {

Player::Player(const std::string name, EventScheduler& scheduler)
    : name_(name),
      scheduler_(scheduler)
{
    procHandle_ = scheduler_.createProcessor<PlayerSM>(std::ref(*this));
}

Player::~Player(void)
{
}

void Player::reset(void)
{
    unplayedPile_.clear();
    playedPile_.clear();

    EvReset evReset;
    scheduler_.queueEvent(procHandle_, evReset);
}

void Player::action(void)
{
    EvAction evAction;
    scheduler_.queueEvent(procHandle_, evAction);
}

Card& Player::evalCard(void)
{
    if (activeRoundCards_.size() <= evalCard_)
    {
        std::cout << "bogus" << std::endl;
    }
    assert(activeRoundCards_.size() > evalCard_);
    return activeRoundCards_[evalCard_];
}

void Player::tie(void)
{
    EvTie evTie;
    scheduler_.queueEvent(procHandle_, evTie);
}

void Player::won()
{
    EvWon evWon;
    scheduler_.queueEvent(procHandle_, evWon);
}

std::vector<Card> Player::lost(void)
{
    auto retVal = activeRoundCards_;

    EvLost evLost;
    scheduler_.queueEvent(procHandle_, evLost);

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

    EvAcceptCards event;
    scheduler_.queueEvent(procHandle_, event);
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

    std::cout << "player " << player.name() << " played ";
    card.print();

    card.flip(face);
    player.activeRoundCards_.push_back(card);
    notifyObservers(player, Player::EV_CARD_PLAYED);
    notifyObservers(player, Player::EV_CARDS_CHANGED);
}

void startTimer(Player& player, const uint32_t ms)
{
    player.timerHandle_ = player.scheduler_.startTimer(player.procHandle_, ms);
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
