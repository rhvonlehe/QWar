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

Round::Round(std::vector<std::shared_ptr<Player> >& players)
    : _players(players),
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

#if 0 // todo remove
    auto it = std::find(_playersActive.begin(), _playersActive.end(), player);

    assert(it != _playersActive.end());
    _playersActive.erase(it);

    if (allPlayersWaiting())
    {
        evaluate();
    }
#endif

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
        auto winner = _players.front();

        for (auto loser : _losers)
        {
            loser->lost(winner);
        }

        winner->won();
    }



#if 0 // old - todo remove
    // Always use the list of winners if it's non-empty
    auto playersToEval = (_winners.size()) ? _winners : _players;
    std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Card>>> allHands;

    for (const auto& p : playersToEval)
    {
        allHands.push_back(std::make_pair(p, p->evalCard()));
    }

    _winners = findWinner(allHands);

    if (_winners.size() > 1)
    {
        for (auto winner : _winners)
        {
            winner->tie();
        }

        _scheduler.queue_event(_processor,
                               boost::intrusive_ptr<EvTie>(new EvTie()));
    }
    else // exactly 1 winner
    {
        std::vector<std::shared_ptr<Card>> roundCards;
        for (const auto& p : _players)
        {
            auto playerCards = p->getActiveRoundCards();
            roundCards.insert(roundCards.end(), playerCards.begin(), playerCards.end());
        }

        auto winningPlayer = _winners.front();
        winningPlayer->won(roundCards);

        _players.erase(std::remove(_players.begin(), _players.end(), winningPlayer), _players.end());

        for (auto& p : _players)
        {
            p->lost();
        }
    }
#endif
}


#if 0
void Round::play()
{
    std::vector<std::shared_ptr<Player>> winners;

    // Most of the time, this is the only call made here
    //
    winners = playNormal();


    while (winners.size() > 1)
    {
        winners = playWar(winners);
    }

    winners[0]->acceptNewCards(Player::PLAYED, _cardsInRound);
}
#endif

#if 0 // todo
bool Round::playNormal(std::shared_ptr<Player>& player)
{
    static std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Card>>> played;

    auto card = player->playCard();
    _cardsInRound.push_back(card);
    played.push_back(std::make_pair(player, card));
    _numPlayersToPlay--;

    if (0 == _numPlayersToPlay)
    {
        _winners = findWinner(played);
        return true;
    }

    return false;
}
#endif

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

#if 0 // old - remove
    _winners.clear();
    std::sort(played.begin(), played.end(), greaterPair);

    for (auto pair : played)
    {
        std::cout << "Player " << pair.first->name();
        std::cout << " played " << pair.second->str() << std::endl;
    }


    // Iterate until the first card that is a lower value and truncate from
    // there on, return the results which will be all Players with the
    // highest card value
    //
    auto highest = played[0];

    for (auto& item : played)
    {
        if (item.second == highest.second)
        {
            _winners.push_back(item.first);
        }
        else
        {
            break;
        }
    }

    return _winners;
#endif
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

#if 0 // todo
std::vector<std::shared_ptr<Player> > Round::playNormal()
{
    // NOTE: in this function we have all active players involved,
    // so when one player can't lay a card, he can be removed
    // from the global list easily.
    //
    std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Card>>> played;

    auto it = std::begin(_players);

    while (it != std::end(_players))
    {
        auto player = (*it);
        if (!player->outOfCards())
        {
            auto card = player->playCard();
            _cardsInRound.push_back(card);
            played.push_back(std::make_pair(player, card));
        }
        else
        {
            _players.erase(it);
        }
        ++it;
    }

    return findWinner(played);
}
#endif

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
#endif

void Round::removePlayer(std::shared_ptr<Player> player)
{
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it != _players.end())
    {
        _players.erase(it);
    }
}


