#include "QWarApplication.h"

QWarApplication::QWarApplication(int argc, char *argv[])
    : QGuiApplication(argc, argv)
{
    _qmlAppEngine = new QQmlApplicationEngine(this);
    _qmlAppEngine->load(QUrl("qrc:/qml/RootWindow.qml"));

}
