#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class QWarApplication : public QGuiApplication
{
public:
    QWarApplication(int argc, char *argv[]);


private:
    QQmlApplicationEngine* _qmlAppEngine = nullptr;
};

