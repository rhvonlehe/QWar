#include "QPlayer.h"

QPlayer::QPlayer(std::shared_ptr<Player> player, QObject *parent)
    : QObject(parent),
      _player(player)
{

}

uint8_t QPlayer::getUnplayedCardCnt() const
{
    return _unplayedCardCnt;
}
