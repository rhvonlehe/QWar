#include "QPlayer.h"

QPlayer::QPlayer(std::shared_ptr<Player> player, QObject *parent)
    : QObject(parent),
      _player(player)
{
    // Register some actions with the player model
    _player->addObserverCallback([&](Player::ObservableEvent event) { update(event); });
}

void QPlayer::playCard()
{
    printf("Player %s played a card\n", getName().toStdString().c_str());
    fflush(stdout);
    _player->playCard();
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

void QPlayer::update(Player::ObservableEvent event)
{
    switch (event)
    {
    case Player::EV_PLAYER_WAITING:
        _active = false;
        emit activeChanged(_active);
        break;
    case Player::EV_CARDS_CHANGED:
        emit unplayedCardCntChanged(getUnplayedCardCnt());
        emit playedCardCntChanged(getPlayedCardCnt());
        break;

    }

}
