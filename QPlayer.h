#ifndef QPLAYER_H
#define QPLAYER_H

#include "Player.h"
#include <QObject>

class QPlayer : public QObject
{
    Q_OBJECT
public:
    explicit QPlayer(std::shared_ptr<Player> player, QObject *parent = nullptr);

    Q_PROPERTY(uint8_t unplayedCardCnt READ getUnplayedCardCnt NOTIFY unplayedCardCntChanged);

    uint8_t getUnplayedCardCnt(void) const;


signals:
    void unplayedCardCntChanged(uint32_t);

private:
    std::shared_ptr<Player> _player;
    uint8_t                 _unplayedCardCnt;
};

#endif // QPLAYER_H
