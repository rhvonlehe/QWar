#include "Round.h"
#include "RoundState.h"
#include <functional>
#include <algorithm>
#include <iostream>

namespace gameplay {

Round::Round(std::vector<Player *>& players,
             EventScheduler& scheduler,
             const std::function<void (ObservableEvent)> callback) :
    players_(players),
    scheduler_(scheduler),
    observerFunc_(callback)
{
    procHandle_ = scheduler_.createProcessor<RoundSM>(std::ref(*this));
}

Round::~Round(void)
{
}

void Round::playerWaiting(Player* player)
{
    EvPlayerWaiting event(player);
    scheduler_.queueEvent(procHandle_, event);
}

void Round::playerEliminated(Player* player)
{
    std::cout << "in Round::playerEliminated" << std::endl;

    EvPlayerEliminated event(player);
    scheduler_.queueEvent(procHandle_, event);
}

void Round::winnerReqCards(Player* player)
{
    std::cout << "in Round::winnerReqCards" << std::endl;
    EvDistributeCards event(player);

    scheduler_.queueEvent(procHandle_, event);
}

int Round::activePlayers(void) const
{
    return players_.size();
}

void Round::handlePlayerWaiting(Player* player)
{
    std::cout << "in Round::handlePlayerWaiting" << std::endl;
    assert(players_.size());

    // Guaranteed that each player only does this once, so just count up to the total
    // player count
    if (++playersWaiting_ == players_.size())
    {
        std::cout << playersWaiting_ <<" players waiting" << std::endl;
        evaluate();
    }
}

void Round::handlePlayerEliminated(Player* player)
{
    std::cout << "in Round::handlePlayerEliminated" << std::endl;
    assert(players_.size());

    // It's possible due to timing that this event came from a previous round
    // and that player was removed when the new round started. Check if player
    // is in the players_ list and if not, discard and do nothing.
    if (std::find(players_.begin(), players_.end(), player) == players_.end())
    {
        return;
    }

    // remove this player from _players, add to _losers. Need to check each
    // operation to make sure it's valid first, though.
    if (std::find(losers_.begin(), losers_.end(), player) != losers_.end())
    {
        losers_.push_back(player);
    }
    players_.erase(std::remove(players_.begin(), players_.end(), player), players_.end());

    std::cout << playersWaiting_ << " players waiting" << std::endl;
    evaluate();
}


void Round::evaluate(void)
{
    // Idea: keep winners in the _players vector and move losers into a
    // loser vector.  This way the evaluation will always be on remaining players.
    findWinner();

    // Winner(s) remain in _players vector
    if (players_.size() > 1)
    {
        playersWaiting_ = 0;

        for (auto winner : players_)
        {
            winner->tie();
        }
    }
    else // exactly 1 winner
    {
        printf("one winner found");
        auto winner = players_.front();

        EvWinner event(winner);
        scheduler_.queueEvent(procHandle_, event);
    }
}

void Round::findWinner(void)
{
    // max_element should be more efficient than doing a full sort
    auto maxIt = std::max_element(players_.begin(),
                                  players_.end(),
                                  [](Player* p1, Player* p2)
                                  { return (p1->evalCard() < p2->evalCard()); } );

    auto highestValue = (*maxIt)->evalCard();

    auto backHalfIt = std::stable_partition(players_.begin(),
                                            players_.end(),
                                            [highestValue](Player* p)
                                            { return (highestValue == p->evalCard()); } );

    losers_.insert(losers_.end(), backHalfIt, players_.end());
    players_.erase(backHalfIt, players_.end());
}

void Round::cullLoserList(void)
{
    // This now culls loser list only
    losers_.erase(std::remove_if(
                      losers_.begin(), losers_.end(),
                      [](const Player* p)
                      {
                          return p->outOfCards();
                      }), losers_.end());
}

void Round::initializeRound(void)
{
    assert(players_.size());

    playersWaiting_ = 0;
    cullLoserList();

    // Put losers back with players
    players_.insert(players_.end(), losers_.begin(), losers_.end());
    losers_.clear();

    std::cout << "players still playing: " << players_.size() << std::endl;

    if (1 == players_.size())
    {
        observerFunc_(EV_ROUND_ONE_PLAYER_LEFT);
    }
}

void Round::distributeCards(Player* winner)
{
    assert(players_.size());
    std::vector<Card> allLoserCards;

    for (auto loser : losers_)
    {
        auto oneLoserCards = loser->lost();
        allLoserCards.insert(allLoserCards.end(), oneLoserCards.begin(), oneLoserCards.end());
    }

    winner->acceptRoundCards(Player::PILE_PLAYED, allLoserCards);

    // Notify the game object the round is done.
    observerFunc_(EV_ROUND_COMPLETE);
}

} // gameplay
