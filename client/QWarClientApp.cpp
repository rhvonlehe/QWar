#include "QWarClientApp.h"
#include <qqmlcontext.h>

QWarClientApp::QWarClientApp(int argc, char *argv[])
    : QGuiApplication(argc, argv)
{
    _qmlAppEngine = std::make_unique<QQmlApplicationEngine>(this);
    _qmlAppEngine->load(QUrl("qrc:/QWarClient/qml/RootWindow.qml"));

    _qmlAppEngine->rootContext()->setContextProperty("serverSocket", &serverSocket_);
}
