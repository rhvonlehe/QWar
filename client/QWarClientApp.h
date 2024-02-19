#pragma once

#include "ServerCtrlSocketAdapter.h"
#include <QGuiApplication>
#include <QtQml/qqmlapplicationengine.h>


class QWarClientApp : public QGuiApplication
{
public:
    QWarClientApp(int argc, char *argv[]);


private:
    std::unique_ptr<QQmlApplicationEngine>  _qmlAppEngine;
    ServerCtrlSocketAdapter                 serverSocket_;
};

