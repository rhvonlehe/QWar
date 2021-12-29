#pragma once

#include "Player.h"
#include <QObject>

class QPlayer : public QObject
{
    Q_OBJECT
public:
    explicit QPlayer(std::shared_ptr<Player> player, QObject *parent = nullptr);
    ~QPlayer(void) = default;

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(bool active READ getActive NOTIFY activeChanged);
    Q_PROPERTY(unsigned int unplayedCardCnt READ getUnplayedCardCnt NOTIFY unplayedCardCntChanged);
    Q_PROPERTY(unsigned int playedCardCnt READ getPlayedCardCnt NOTIFY playedCardCntChanged);
    Q_PROPERTY(QString playedCards READ getPlayedCards NOTIFY playedCardsChanged);

    Q_INVOKABLE void playCard(void);

    QString getName(void) const;
    bool getActive(void) const { return _active; }
    void setName(QString name);
    uint8_t getUnplayedCardCnt(void) const;
    uint8_t getPlayedCardCnt(void) const;
    QString getPlayedCards(void);
    void update(Player::ObservableEvent event);


signals:
    void nameChanged(QString);
    void activeChanged(bool);
    void unplayedCardCntChanged(uint32_t);
    void playedCardCntChanged(uint32_t);
    void playedCardsChanged(QString);


private:
    std::shared_ptr<Player> _player;
    bool                    _active;
    QString                 _cardsPlayed; // will change later to look better
};
