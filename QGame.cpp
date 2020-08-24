#include "QGame.h"
#include <Player.h>

QGame::QGame(QObject *parent) : QObject(parent)
{

}

void QGame::addPlayer(const QString& newPlayer)
{
    _players.push_back(Player(newPlayer.toStdString()));
}

void QGame::start(void)
{
    _game = std::make_unique<Game>(_players);
    _game->play();
}

void QGame::quit(void)
{
    emit gameQuit();
}
