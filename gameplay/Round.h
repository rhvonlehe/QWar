#pragma once

#include <Player.h>
#include <vector>
#include <memory>

struct WarHand
{
    WarHand() = default;
    WarHand(std::shared_ptr<Player> p, std::shared_ptr<Card> c1, std::shared_ptr<Card> c2)
        : player(p), downCard(c1), upCard(c2)
    {
    }
    std::shared_ptr<Player> player;
    std::shared_ptr<Card>   downCard;
    std::shared_ptr<Card>   upCard;
};


class Round
{
public:
    Round(std::vector<std::shared_ptr<Player>>& players); // todo remove
    Round(unsigned int playerCnt);
    void play();
    bool playNormal(std::shared_ptr<Player>& player);
private:
    std::vector<std::shared_ptr<Player>> findWinner(std::vector<std::pair<std::shared_ptr<Player>,
                                    std::shared_ptr<Card>>>& played);
    std::vector<std::shared_ptr<Player> > findWinner(std::vector<WarHand>& played);
    std::vector<std::shared_ptr<Player>> playNormal();
    std::vector<std::shared_ptr<Player> > playWar(std::vector<std::shared_ptr<Player> > &players);
    void removePlayer(std::shared_ptr<Player> player);


    unsigned int                            _numPlayersToPlay;
    std::vector<std::shared_ptr<Player>>    _winners;
    std::vector<std::shared_ptr<Player>>    _players;
    std::vector<std::shared_ptr<Card>>      _cardsInRound;

};
