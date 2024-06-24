#include "Round.h"
#include "RoundState.h"
#include <functional>
#include <algorithm>
#include <iostream>

Round::Round(std::vector<Player *>& players,
             const std::function<void ()> callback)
    : _players(players),
    _observerFunc(callback),
    _scheduler(true)
{
    // Set up event processor
    _processor = _scheduler.create_processor<RoundSM>(std::ref(*this));
    _scheduler.initiate_processor(_processor);

    _processorThread =
        std::make_unique<std::thread>(
            [&]()    {
                std::cout << "starting round SM thread" << std::endl;
                _scheduler();
            }
            );
}

Round::~Round(void)
{
    _scheduler.terminate();
    _processorThread->join();
}

void Round::playerWaiting(Player* player)
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvPlayerWaiting>(new EvPlayerWaiting(player)));
}

void Round::playerEliminated(Player* player)
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvPlayerEliminated>(new EvPlayerEliminated(player)));
}

void Round::winnerReqCards(Player* player)
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvDistributeCards>(new EvDistributeCards(player)));
}

void Round::handlePlayerWaiting(Player* player)
{
    assert(_players.size());

    // Guaranteed that each player only does this once, so just count up to the total
    // player count
    if (++_playersWaiting == _players.size())
    {
        std::cout << _playersWaiting <<" players waiting" << std::endl;
        evaluate();
    }
}

void Round::handlePlayerEliminated(Player* player)
{
    assert(_players.size());

    // remove this player from _players, add to _losers
    _losers.push_back(player);
    _players.erase(std::remove(_players.begin(), _players.end(), player));
}


void Round::evaluate(void)
{
    // Idea: keep winners in the _players vector and move losers into a
    // loser vector.  This way the evaluation will always be on remaining players.

    findWinner();

    // Winner(s) remain in _players vector
    if (_players.size() > 1)
    {
        _playersWaiting = 0;

        for (auto winner : _players)
        {
            winner->tie();
        }
    }
    else // exactly 1 winner
    {
        printf("one winner found");
        auto winner = _players.front();

        _scheduler.queue_event(_processor,
                               boost::intrusive_ptr<EvWinner>(new EvWinner(winner)));
    }
}

void Round::findWinner(void)
{
    // max_element should be more efficient than doing a full sort
    auto maxIt = std::max_element(_players.begin(),
                                  _players.end(),
                                  [](Player* p1, Player* p2)
                                  { return (p1->evalCard() < p2->evalCard()); } );

    auto highestValue = (*maxIt)->evalCard();

    auto backHalfIt = std::stable_partition(_players.begin(),
                                            _players.end(),
                                            [highestValue](Player* p)
                                            { return (highestValue == p->evalCard()); } );

    _losers.insert(_losers.end(), backHalfIt, _players.end());
    _players.erase(backHalfIt, _players.end());
}

void Round::cullLoserList(void)
{
    // This now culls loser list only
    _losers.erase(std::remove_if(
                      _losers.begin(), _losers.end(),
                      [](const Player* p)
                      {
                          return p->outOfCards();
                      }), _losers.end());
}

void Round::initializeRound(void)
{
    assert(_players.size());

    _playersWaiting = 0;
    cullLoserList();

    // Put losers back with players
    _players.insert(_players.end(), _losers.begin(), _losers.end());
    _losers.clear();
}

void Round::distributeCards(Player* winner)
{
    assert(_players.size());
    std::vector<Card> allLoserCards;

    for (auto loser : _losers)
    {
        auto oneLoserCards = loser->lost();
        allLoserCards.insert(allLoserCards.end(), oneLoserCards.begin(), oneLoserCards.end());
    }

    winner->acceptRoundCards(Player::PILE_PLAYED, allLoserCards);

    // Notify the game object the round is done.
    _observerFunc();
}

