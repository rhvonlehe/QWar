#include <QGameModel.h>
#include <iostream>

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
        _players.push_back(new QPlayer(player));
    }

    _game = std::make_unique<Game>(players);
}

void QGameModel::deal(void)
{
    _game->deal();

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
    std::cout << "Resetting gameModel" << std::endl;
    _players.clear();
    _game = nullptr;
    _roundNumber = 0;
}


// todo - make the parameter the qobject (or QPlayer?) passed in from QML
// also increment the players that have played cards to know when done to reset
void QGameModel::playCard(QPlayer* player)
{



#if 0 // todo old
    std::cout << "player calling playCard is " << player->getName().toStdString() << std::endl;

    static bool roundComplete = true;
    static int playersLeft = 0;

    decltype(_currentRound->playNormal()) winners;

    if (roundComplete)
    {
        playersLeft = _game->players().size();
        _roundNumber++;
        _currentRound = std::make_unique<Round>(_game->players());

        winners = _currentRound->playNormal();
    }

    player->playCard();
    playersLeft--;

    if (0 == playersLeft)
    {
        roundComplete = true;


    }
#endif
}

#if 0 // todo
void QGameMode::playWar(void)
{
    while (winners.size() > 1)
    {
        winners = round.playWar(winners);
    }

    _winner
}
#endif


int QGameModel::rowCount(const QModelIndex &parent) const
{
    return _players.size();
}

QVariant QGameModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role);
    return QVariant::fromValue(_players[index.row()]);
}

QHash<int, QByteArray> QGameModel::roleNames() const
{
    static QHash<int, QByteArray> *pHash = nullptr;
    if (!pHash)
    {
        pHash = new QHash<int, QByteArray>;
        (*pHash)[Qt::UserRole + 1] = "player";
    }

    return *pHash;
}



//QHash<int, QByteArray> QGameModel::roleNames() const
//{

//}
