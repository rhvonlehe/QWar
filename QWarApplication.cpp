#include "QWarApplication.h"
#include <QQmlContext>
#include <memory>

QWarApplication::QWarApplication(int argc, char *argv[])
    : QGuiApplication(argc, argv)
{
    _qmlAppEngine = std::make_unique<QQmlApplicationEngine>(this);
    _qmlAppEngine->load(QUrl("qrc:/qml/RootWindow.qml"));

    _qmlAppEngine->rootContext()->setContextProperty("gameController", &_gameController);
    _qmlAppEngine->rootContext()->setContextProperty("gameModel", &_gameModel);

    QObject::connect(&_gameController, &QGameController::gameQuit, this, &QWarApplication::onGameQuit);
}

void QWarApplication::onGameQuit(void)
{
    QCoreApplication::exit();
}
