#pragma once

#include <Player.h>
#include <vector>
#include <memory>

struct WarHand
{
    WarHand() = default;
    WarHand(Player* p, std::shared_ptr<Card> c1, std::shared_ptr<Card> c2)
        : player(p), downCard(c1), upCard(c2)
    {
    }
    Player*                 player;
    std::shared_ptr<Card>   downCard;
    std::shared_ptr<Card>   upCard;
} ;


class Round
{
public:
    Round(std::vector<Player>& players);
    void play();
private:
    std::vector<Player*> findWinner(std::vector<std::pair<Player*,
                                    std::shared_ptr<Card>>>& played);
    std::vector<Player*> findWinner(std::vector<WarHand>& played);
    std::vector<Player*> playNormal();
    std::vector<Player*> playWar(std::vector<Player *> &players);
    void removePlayer(Player *player);

    std::vector<Player>& _players;
    std::vector<std::shared_ptr<Card>>    _cardsInRound;

};
