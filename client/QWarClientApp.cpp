#include "QWarClientApp.h"
#include <qqmlcontext.h>

QWarClientApp::QWarClientApp(int argc, char *argv[])
    : QGuiApplication(argc, argv)
{
    qmlAppEngine_ = std::make_unique<QQmlApplicationEngine>(this);
    qmlAppEngine_->load(QUrl("qrc:/QWarClient/qml/RootWindow.qml"));

    qmlAppEngine_->rootContext()->setContextProperty("clientAdapter", &clientAdapter_);
}
