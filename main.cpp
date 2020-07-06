#include <Game.h>
#include <QWarApplication.h>
#include <time.h>
#include <vector>



int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(nullptr)));
    QWarApplication app(argc, argv);

#if 0
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
#endif
    return app.exec();
}
