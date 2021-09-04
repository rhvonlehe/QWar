#pragma once

#include <Game.h>

#include <QPlayer.h>
#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <memory>


class QGameModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataType
    {

    };

    QGameModel();
    Q_INVOKABLE void        populate(const QStringList& nameList);

    Q_INVOKABLE void        deal(void);
    void                    reset(void);



    virtual int rowCount(const QModelIndex& parent=QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
private:

    std::vector<std::unique_ptr<QPlayer>>   _players;
    std::unique_ptr<Game>                   _game;
};

