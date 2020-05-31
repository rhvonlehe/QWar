#pragma once

#include <Player.h>
#include <vector>
#include <memory>

struct WarHand
{
    WarHand() = default;
    WarHand(Player* p, Card c1, Card c2)
        : player(p), downCard(c1), upCard(c2)
    {
    }
    Player* player;
    Card   downCard;
    Card   upCard;
} ;


class Round
{
public:
    Round(std::vector<Player>& players);
    void play();
private:
    std::vector<Player*> findWinner(std::vector<std::pair<Player*, Card>>& played);
    std::vector<Player*> findWinner(std::vector<WarHand>& played);
    std::vector<Player*> playNormal();
    std::vector<Player*> playWar(std::vector<Player *> &players);
    void removePlayer(Player *player);

    std::vector<Player>& _players;
    std::vector<Card>    _cardsInRound;

};
