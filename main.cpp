#include <Game.h>
#include <QGuiApplication>
#include <time.h>
#include <vector>



int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(nullptr)));
    QGuiApplication app(argc, argv);

    std::vector<Player> playerList;

    QStringList arguments = app.arguments();
    arguments.pop_front(); // remove program name

    for (QString item : arguments)
    {
        Player player(item.toUtf8().constData());
        playerList.push_back(player);
    }

    Game game(playerList);
    game.play();

    return app.exec();
}
