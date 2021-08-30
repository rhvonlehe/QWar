#include "QGame.h"
#include <Player.h>

#include <iostream>

QGame::QGame(QObject *parent) : QObject(parent)
{

}

void QGame::reset(void)
{
    _playerNames.clear();
    _game = nullptr;
    _players.clear();
}

void QGame::setPlayers(const QStringList &playerList)
{
    QStringList temp = playerList;

    for (auto& item : temp)
    {
        std::cout << "player name is: " << item.toStdString() << std::endl;
    }
}

void QGame::addPlayer(const QString& newPlayer)
{
    _playerNames.push_back(newPlayer);
//    _players.push_back(Player(newPlayer.toStdString()));
}

QStringList QGame::getPlayers(void)
{
    QStringList retVal;

    for (const auto& player : _players)
    {
//        retVal.append(QString::fromStdString(player->name()));
    }

    return retVal;
}

void QGame::start(void)
{
    // populate the model with what we have, it will be updated as
    // cards are dealt, etc


    // todo
//    _game->play();
}

void QGame::quit(void)
{
    emit gameQuit();
}

void QGame::deal(void)
{      
    // TODO: Consider not doing all of this here.
    // Collect names and create QPlayer and Player objects
    // Each QPlayer has a reference to the corresponding Player that the GamePlay lib owns
    std::vector<std::shared_ptr<Player>> players;

    for (const auto& name: _playerNames)
    {
        auto newPlayer = std::make_shared<Player>(name.toStdString());
//        QPlayer player(newPlayer);
//        _players.push_back(player);
        _players.emplace_back(std::make_unique<QPlayer>(newPlayer));
    }

    _game = std::make_unique<Game>(players);
}
