#include "QGameModel.h"

QGameModel::QGameModel()
{

}

void QGameModel::populate(const QStringList& nameList)
{
    std::vector<std::shared_ptr<Player>> players;

    for (const auto& name : nameList)
    {
        auto player = std::make_shared<Player>(name.toStdString());
        players.push_back(player);
        auto qPlayer = std::make_unique<QPlayer>(player);
        _players.push_back(std::move(qPlayer));
    }

    _game = std::make_unique<Game>(players);
}

void QGameModel::deal(void)
{
#if 0
    // TODO: Consider not doing all of this here.
    // Collect names and create QPlayer and Player objects
    // Each QPlayer has a reference to the corresponding Player that the GamePlay lib owns
    std::vector<std::shared_ptr<Player>> players;

    for (const auto& name: _playerNames)
    {
        auto newPlayer = std::make_shared<Player>(name.toStdString());
        //        QPlayer player(newPlayer);
        //        _players.push_back(player);
        _players.emplace_back(std::make_unique<QPlayer>(newPlayer));
    }

    _game = std::make_unique<Game>(players);
#endif
}

void QGameModel::reset(void)
{
    _players.clear();
}


int QGameModel::rowCount(const QModelIndex &parent) const
{

    //temp
    return 0;
}

QVariant QGameModel::data(const QModelIndex& index, int role) const
{

    return QVariant(0);
}

QHash<int, QByteArray> QGameModel::roleNames() const
{
    QHash<int, QByteArray>  d;
//    d[] = ;

    return d;
}



//QHash<int, QByteArray> QGameModel::roleNames() const
//{

//}
