#pragma once

#include <Game.h>
#include <Round.h>
#include <QPlayer.h>
#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <memory>

class QGameModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleType
    {
        ROLE_NAME = Qt::UserRole + 1,
        ROLE_PLAYED_CARD,
        ROLE_PLAYED_CNT,
        ROLE_UNPLAYED_CNT
    };

    QGameModel();
    Q_INVOKABLE void        populate(const QStringList& nameList);

    Q_INVOKABLE void        deal(void);
    Q_INVOKABLE void        reset(void);
    Q_INVOKABLE void        playCard(QPlayer *player);


    virtual int rowCount(const QModelIndex& parent=QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
private:

    // Must store ptrs to QObject-derived type.  They are not copyable.
    std::vector<QPlayer*>           _players;
    std::unique_ptr<Game>           _game;
    int                             _roundNumber;
    std::unique_ptr<Round>          _currentRound;

//    std::map<std::string, std::shared_ptr<Player>> _playerMap; // todo remove
};


