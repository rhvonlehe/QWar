#include "Round.h"
#include <algorithm>
#include <iostream>

struct
{
    bool operator()(std::pair<Player*, Card> a, std::pair<Player*, Card> b) const
    {
        return a.second > b.second;
    }

} greaterPair;

struct
{
    bool operator()(WarHand a, WarHand b) const
    {
        return a.downCard > b.downCard;
    }
} greaterWarHand;

Round::Round(std::vector<Player>& players)
    : _players(players)
{

}

void Round::play()
{
    std::vector<Player*> winners;

    // Most of the time, this is the only call made here
    //
    winners = playNormal();


    while (winners.size() > 1)
    {
        winners = playWar(winners);
    }

    winners[0]->acceptNewCards(Player::PLAYED, _cardsInRound);
}

std::vector<Player*> Round::findWinner(std::vector<std::pair<Player*, Card>>& played)
{
    std::vector<Player*> winners;

    std::sort(played.begin(), played.end(), greaterPair);

    for (auto pair : played)
    {
        std::cout << "Player " << pair.first->name();
        std::cout << " played " << pair.second.str() << std::endl;
    }


    // Iterate until the first card that is a lower value and truncate from
    // there on, return the results which will be all Players with the
    // highest card value
    //
    std::pair<Player*, Card> highest = played[0];

    for (auto& item : played)
    {
        if (item.second == highest.second)
        {
            winners.push_back(item.first);
        }
        else
        {
            break;
        }
    }

    return winners;
}

std::vector<Player*> Round::findWinner(std::vector<WarHand> &played)
{
    std::vector<Player*> winners;

    std::sort(played.begin(), played.end(), greaterWarHand);

    for (auto pair : played)
    {
        std::cout << "Player " << pair.player->name();
        std::cout << " played " << pair.downCard.str() << std::endl;
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

std::vector<Player*> Round::playNormal()
{
    // NOTE: in this function we have all active players involved,
    // so when one player can't lay a card, he can be removed
    // from the global list easily.
    //
    std::vector<std::pair<Player*, Card>> played;

    std::vector<Player>::iterator it = std::begin(_players);

    while (it != std::end(_players))
    {
        Player* player = &(*it);
        if (!player->outOfCards())
        {
            Card card = player->playCard();
            _cardsInRound.push_back(card);
            played.push_back(std::make_pair(player, card));
        }
        else
        {
            _players.erase(it);
        }
        ++it;
    }

    std::vector<Player*> winners = findWinner(played);

    return winners;
}

std::vector<Player*> Round::playWar(std::vector<Player*>& players)
{
    // NOTE: in this function, potentially only a subset of all active
    // players are involved.  When a player can not lay enough cards,
    // they must still be removed from the overall player list, but
    // it is somewhat harder to do this
    //
    std::vector<WarHand> played;

    for (Player* player : players)
    {
        if (player->hasTwoCards())
        {
            Card downCard = player->playCard();
            Card upCard = player->playCard();
            WarHand hand(player, downCard, upCard);
            _cardsInRound.push_back(downCard);
            _cardsInRound.push_back(upCard);
            played.push_back(hand);
        }
        else
        {
            // TODO: remove this player from the active player list,
            // but force him to forfeit any single card he may still have
            //
            removePlayer(player);
        }
    }

    std::vector<Player*> winners = findWinner(played);

    return winners;
}

void Round::removePlayer(Player* player)
{
    _players.erase(std::remove(_players.begin(), _players.end(), *player), _players.end());
}


