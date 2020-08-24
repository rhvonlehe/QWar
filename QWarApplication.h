#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QGame.h>

class QWarApplication : public QGuiApplication
{
public:
    QWarApplication(int argc, char *argv[]);

public slots:
    void onGameQuit(void);
private:
    std::unique_ptr<QQmlApplicationEngine> _qmlAppEngine;
    QGame _game;
};

