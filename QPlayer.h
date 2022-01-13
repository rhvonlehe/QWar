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
    Q_PROPERTY(bool winner READ getWinner NOTIFY winnerChanged);
    Q_PROPERTY(QString buttonText READ getButtonText NOTIFY buttonTextChanged)
    Q_PROPERTY(unsigned int unplayedCardCnt READ getUnplayedCardCnt NOTIFY unplayedCardCntChanged);
    Q_PROPERTY(unsigned int playedCardCnt READ getPlayedCardCnt NOTIFY playedCardCntChanged);
    Q_PROPERTY(QString playedCards READ getPlayedCards NOTIFY playedCardsChanged);

    Q_INVOKABLE void action(void);

    bool getActive(void) const { return _active; }
    bool getWinner(void) const { return _identifyWinner; }
    QString getButtonText(void) const;
    QString getName(void) const;
    void setName(QString name);
    uint8_t getUnplayedCardCnt(void) const;
    uint8_t getPlayedCardCnt(void) const;
    QString getPlayedCards(void);

signals:
    void nameChanged(QString);
    void activeChanged(bool);
    void winnerChanged(bool);
    void buttonTextChanged(QString);
    void unplayedCardCntChanged(quint8);
    void playedCardCntChanged(quint8);
    void playedCardsChanged(QString);

private:
    void updateCardsPlayed(void);
    void update(Player::ObservableEvent event);

private:

    std::shared_ptr<Player> _player;
    bool                    _active;
    bool                    _identifyWinner;
    QString                 _cardsPlayed; // will change later to look better
};
