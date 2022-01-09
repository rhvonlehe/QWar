#include "Round.h"
#include "RoundState.h"
#include <functional>
#include <algorithm>
#include <iostream>

struct
{
    bool operator()(std::pair<std::shared_ptr<Player>, std::shared_ptr<Card>> a,
                    std::pair<std::shared_ptr<Player>, std::shared_ptr<Card>> b) const
    {
        return *(a.second) > *(b.second);
    }

} greaterPair;

struct
{
    bool operator()(WarHand a, WarHand b) const
    {
        return *(a.downCard) > *(b.downCard);
    }
} greaterWarHand;

Round::Round(std::vector<std::shared_ptr<Player> >& players,
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
                [&]()
    {
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

void Round::playerWaiting(std::shared_ptr<Player> player)
{
    _scheduler.queue_event(_processor,
                           boost::intrusive_ptr<EvPlayerWaiting>(new EvPlayerWaiting(player)));
}

void Round::handlePlayerWaiting(std::shared_ptr<Player> player)
{
    // Guaranteed that each player only does this once, so just count up to the total
    // player count
    static int playersWaiting = 0;

    if (++playersWaiting == _players.size())
    {
        evaluate();
    }
}

void Round::evaluate(void)
{
    // Idea: keep winners in the _players vector and move losers into a
    // loser vector.  This way the evaluation will always be on remaining players.

    findWinner();

    // Winner(s) remain in _players vector
    if (_players.size() > 1)
    {
        for (auto winner : _players)
        {
            winner->tie();
        }

        _scheduler.queue_event(_processor,
                               boost::intrusive_ptr<EvTie>(new EvTie()));
    }
    else // exactly 1 winner
    {
        _players.front()->won();

#if 0 // todo move elsewhere
        std::vector<std::shared_ptr<Card>> allLoserCards;

        for (auto loser : _losers)
        {
            auto oneLoserCards = loser->lost();
            allLoserCards.insert(allLoserCards.end(), oneLoserCards.begin(), oneLoserCards.end());
        }

        winner->won(allLoserCards);

        // Notify the game object the round is done.
        _observerFunc();
#endif
    }
}

void Round::findWinner(void)
{
    // Result has winners left in _players and losers in _losers
    //
    auto sorter = [](std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
        return (*p1->evalCard() > *p2->evalCard());
    };

    // max_element should be more efficient than doing a full sort
    auto maxIt = std::max_element(_players.begin(),
                                  _players.end(),
                                  [](std::shared_ptr<Player> p1, std::shared_ptr<Player> p2)
    { return (*p1->evalCard() < *p2->evalCard()); } );

    auto highestValue = *(*maxIt)->evalCard();

    //    std::sort(_players.begin(), _players.end(), sorter);
    //    auto highestValue = *_players.front()->evalCard();

    auto backHalfIt = std::stable_partition(_players.begin(),
                                            _players.end(),
                                            [highestValue](std::shared_ptr<Player> p)
    { return (highestValue == *p->evalCard()); } );

    _losers.insert(_losers.end(), backHalfIt, _players.end());
    _players.erase(backHalfIt, _players.end());
}

std::vector<std::shared_ptr<Player>> Round::findWinner(std::vector<WarHand>& played)
{
    std::vector<std::shared_ptr<Player>> winners;

    std::sort(played.begin(), played.end(), greaterWarHand);

    for (auto pair : played)
    {
        std::cout << "Player " << pair.player->name();
        std::cout << " played " << pair.downCard->str() << " down" << std::endl;

        std::cout << "and " << pair.upCard->str() << " up" << std::endl;
    }

    // Iterate until the first card that is a lower value and truncate from
    // there on, return the results which will be all Players with the
    // highest card value
    //
    WarHand highest = played[0];

    for (auto& item : played)
    {
        if (item.downCard == highest.downCard)
        {
            winners.push_back(item.player);
        }
        else
        {
            break;
        }
    }

    return winners;
}



#if 0
std::vector<std::shared_ptr<Player>> Round::playWar(std::vector<std::shared_ptr<Player>>& players)
{
    // NOTE: in this function, potentially only a subset of all active
    // players are involved.  Remove a player from the local player
    // list when they can't complete a war due to lack of cards.
    // Let the game determine who has dropped out after the round
    // has finished playing.
    //
    std::vector<WarHand> played;

    auto it = std::begin(players);

    while (it != std::end(players))
    {
        auto player = *it;
        if (player->hasTwoCards())
        {
            auto downCard = player->playCard();
            auto upCard = player->playCard();
            WarHand hand(player, downCard, upCard);
            _cardsInRound.push_back(downCard);
            _cardsInRound.push_back(upCard);
            played.push_back(hand);
            it++;
        }
        else
        {
            // Remove this player from the active player list,
            // but force him to forfeit any single card he may still have
            //
            if (!player->outOfCards())
            {
                _cardsInRound.push_back(player->playCard());
            }
            it = players.erase(it);  // remove from local vector
        }
    }

    std::vector<std::shared_ptr<Player>> winners = findWinner(played);

    return winners;
}

void Round::removePlayer(std::shared_ptr<Player> player)
{
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it != _players.end())
    {
        _players.erase(it);
    }
}
#endif


