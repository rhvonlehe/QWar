#include "QPlayer.h"

QPlayer::QPlayer(std::shared_ptr<Player> player, QObject *parent)
    : QObject(parent),
      _player(player),
      _identifyWinner(false)
{
    // Register some actions with the player model
    _player->addObserverCallback([&](Player::ObservableEvent event) { update(event); });
}

void QPlayer::action()
{
    printf("Player %s hit their button\n", getName().toStdString().c_str());
    fflush(stdout);
    _player->action();
}

QString QPlayer::getButtonText(void) const
{
    // TODO: make this an array indexed by an enum?
    if (_identifyWinner)
    {
        return QString("Accept Cards");
    }
//    else if ()
//    {

//    }
    else {
        return QString("Play Card");
    }
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

QString QPlayer::getPlayedCards(void)
{
    return _cardsPlayed;
}

void QPlayer::update(Player::ObservableEvent event)
{
    switch (event)
    {
    case Player::EV_PLAYER_WAIT_WINNER:
        _active = false;
        emit activeChanged(_active);
        break;
    case Player::EV_PLAYER_ACTIVE:
        _active = true;
        emit activeChanged(_active);
        break;
    case Player::EV_CARD_PLAYED:
        {
            auto cardStr = QString::fromStdString(_player->lastCardPlayed()->str());
            _cardsPlayed.append(cardStr).append(" ");
            emit playedCardsChanged(_cardsPlayed);
        }
        break;
    case Player::EV_CARDS_CHANGED:
        emit unplayedCardCntChanged(getUnplayedCardCnt());
        emit playedCardCntChanged(getPlayedCardCnt());
        break;
    case Player::EV_WINNER:
        _identifyWinner = true;
        emit winnerChanged(_identifyWinner);
        emit buttonTextChanged(getButtonText());
        break;
    case Player::EV_PLAYER_WAIT_FLIP:
        // TODO
        break;
    case Player::EV_WINNER_REQ_CARDS:
        _identifyWinner = false;
        emit winnerChanged(_identifyWinner);
        emit buttonTextChanged(getButtonText());
    default:
        break;
    }

}
