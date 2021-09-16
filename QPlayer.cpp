#include "QPlayer.h"
#include <Round.h>

QPlayer::QPlayer(std::shared_ptr<Player> player, QObject *parent)
    : QObject(parent),
      _player(player)
{

}

void QPlayer::playCard()
{
}

QString QPlayer::getName() const
{
    auto name = _player->name();
    return QString::fromUtf8(name.data(), name.size());
}

void QPlayer::setName(QString name)
{
    _player->name(name.toStdString());
}

uint8_t QPlayer::getUnplayedCardCnt() const
{
    return _player->totalUnplayed();
}

uint8_t QPlayer::getPlayedCardCnt() const
{
    return _player->totalPlayed();
}
