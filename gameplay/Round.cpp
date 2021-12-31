#include "Round.h"
#include "RoundState.h"
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
    : _players(players), _playersWaiting(players)
{
    // Set up event processor
    _processor = _scheduler.create_processor<RoundSM>(this);
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
                           boost::intrusive_ptr<EvPlayerWaiting>(new EvPlayerWaiting()));

    auto it = std::find(_playersWaiting.begin(), _playersWaiting.end(), player);

    assert(it != _playersWaiting.end());
    _playersWaiting.erase(it);


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

std::vector<std::shared_ptr<Player>> Round::findWinner(std::vector<std::pair<std::shared_ptr<Player>,
                                                       std::shared_ptr<Card>>>& played)
{
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


