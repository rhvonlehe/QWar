#include <Game.h>
#include <QWarApplication.h>
#include <time.h>
#include <vector>


int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(nullptr)));
    QWarApplication app(argc, argv);

    return app.exec();
}
