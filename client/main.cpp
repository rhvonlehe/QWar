#include "QWarClientApp.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(nullptr)));
    QWarClientApp app(argc, argv);

    return app.exec();
}
