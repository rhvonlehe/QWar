#pragma once

#include "ClientAdapter.h"
#include <QGuiApplication>
#include <QtQml/qqmlapplicationengine.h>


class QWarClientApp : public QGuiApplication
{
public:
    QWarClientApp(int argc, char *argv[]);


private:
    std::unique_ptr<QQmlApplicationEngine>  qmlAppEngine_;
    ClientAdapter                           clientAdapter_;
};

