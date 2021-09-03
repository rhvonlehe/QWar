#include "QGameController.h"
#include <Player.h>

#include <iostream>

QGameController::QGameController(QObject *parent) : QObject(parent)
{

}

void QGameController::reset(void)
{
}

#if 0 // todo remove
void QGameController::setPlayers(const QStringList &playerList)
{
    QStringList temp = playerList;

    for (auto& item : temp)
    {
        std::cout << "player name is: " << item.toStdString() << std::endl;
    }
}

void QGameController::addPlayer(const QString& newPlayer)
{
    _playerNames.push_back(newPlayer);
//    _players.push_back(Player(newPlayer.toStdString()));
}

QStringList QGameController::getPlayers(void)
{
    QStringList retVal;

    for (const auto& player : _players)
    {
//        retVal.append(QString::fromStdString(player->name()));
    }

    return retVal;
}
#endif

void QGameController::start(void)
{
    // populate the model with what we have, it will be updated as
    // cards are dealt, etc


    // todo
//    _game->play();
}

void QGameController::quit(void)
{
    emit gameQuit();
}


