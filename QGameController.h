#ifndef QGAME_H
#define QGAME_H

#include <QGameModel.h>
#include <QString>
#include <QStringList>
#include <QObject>

class QGameController : public QObject
{
    Q_OBJECT
public:
    explicit QGameController(QObject *parent = nullptr);
    Q_INVOKABLE void        reset(void);
    Q_INVOKABLE void        start(void);
    Q_INVOKABLE void        quit(void);

public slots:

signals:
    void gameQuit(void);


private:
};

#endif // QGAME_H
