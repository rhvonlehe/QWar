#include <QGameModel.h>
#include <iostream>

QGameModel::QGameModel()
{
}

void QGameModel::populate(const QStringList& nameList)
{
    // Create the game, passing the list of strings.
    // The QGameModel needs its own QPlayer objects which need
    // pointers to the non-UI Player, so ask for those pointers
    // after Game object is created.

    std::vector<std::string> playerNames;
    for (const auto& name : nameList)
    {
        playerNames.push_back(name.toStdString());
    }

    _game = std::make_unique<Game>(playerNames);

    for (const auto& name : nameList)
    {
        auto& player = _game->getPlayer(name.toStdString());
        _players.push_back(std::make_unique<QPlayer>(&player));
    }
}

void QGameModel::deal(void)
{
    _game->deal();
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
    return QVariant::fromValue(_players[index.row()].get());
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
