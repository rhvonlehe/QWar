#ifndef QGAME_H
#define QGAME_H

#include <Game.h>
#include <QPlayer.h>
#include <QString>
#include <QStringList>
#include <QObject>
#include <memory>

class QGame : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QStringList players READ players WRITE )
public:
    explicit QGame(QObject *parent = nullptr);
    Q_INVOKABLE void        reset(void);
    Q_INVOKABLE void        setPlayers(const QStringList& playerList);
    Q_INVOKABLE void        addPlayer(const QString& newPlayer);
    Q_INVOKABLE QStringList getPlayers(void);
    Q_INVOKABLE void        start(void);
    Q_INVOKABLE void        quit(void);
    Q_INVOKABLE void        deal(void);

public slots:
//    void handleCardPlayed(QString playerName);

signals:
    void gameQuit(void);


private:
    std::vector<QString>                    _playerNames;
    std::vector<std::unique_ptr<QPlayer>>   _players;
    std::unique_ptr<Game>                   _game;
};

#endif // QGAME_H
