#ifndef QGAME_H
#define QGAME_H

#include <Game.h>
#include <QStringList>
#include <QObject>
#include <memory>

class QGame : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QStringList players READ players WRITE )
public:
    explicit QGame(QObject *parent = nullptr);
    Q_INVOKABLE void        addPlayer(const QString& newPlayer);
    Q_INVOKABLE QStringList getPlayers(void);
    Q_INVOKABLE void        start(void);
    Q_INVOKABLE void        quit(void);


signals:
    void gameQuit(void);

private:
    std::vector<Player> _players;
    std::unique_ptr<Game> _game;
};

#endif // QGAME_H
