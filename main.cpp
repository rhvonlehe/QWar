#include <Game.h>
#include <QCoreApplication>
#include <time.h>
#include <vector>



int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(nullptr)));
    QCoreApplication a(argc, argv);

    std::vector<Player> playerList;

    QStringList arguments = a.arguments();
    arguments.pop_front(); // remove program name

    for (QString item : arguments)
    {
        Player player(item.toUtf8().constData());
        playerList.push_back(player);
    }

    Game game(playerList);
    game.play();

    return a.exec();
}
