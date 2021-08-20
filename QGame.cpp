#include "QGame.h"
#include <Player.h>

QGame::QGame(QObject *parent) : QObject(parent)
{

}

void QGame::reset(void)
{
    _playerNames.clear();
    _game = nullptr;
    _players.clear();
}

void QGame::addPlayer(const QString& newPlayer)
{
    _playerNames.push_back(newPlayer);
//    _players.push_back(Player(newPlayer.toStdString()));
}

QStringList QGame::getPlayers(void)
{
    QStringList retVal;

//    for (const auto& player : _players)
//    {
//        retVal.append(QString::fromStdString(player.name()));
//    }

    return retVal;
}

void QGame::start(void)
{
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
    std::vector<Player> players;

    for (const auto& name: _playerNames)
    {
        players.emplace_back(name.toStdString());
    }

    _game = std::make_unique<Game>(std::move(players));
}
