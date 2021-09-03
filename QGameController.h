#ifndef QGAME_H
#define QGAME_H

#include <QGameModel.h>
#include <QString>
#include <QStringList>
#include <QObject>
#include <memory>

class QGameController : public QObject
{
    Q_OBJECT
    //    Q_PROPERTY(QStringList players READ players WRITE )
public:
    explicit QGameController(QObject *parent = nullptr);
    Q_INVOKABLE void        reset(void);
    Q_INVOKABLE void        start(void);
    Q_INVOKABLE void        quit(void);

    // todo remove
//    QGameModel&             getModel() { return _model; }

public slots:
    //    void handleCardPlayed(QString playerName);

signals:
    void gameQuit(void);


private:
};

#endif // QGAME_H
