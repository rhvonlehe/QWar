#pragma once

#include <QGuiApplication>
#include <QtQml/qqmlapplicationengine.h>
#include <QGameController.h>
#include <QGameModel.h>

class QWarApplication : public QGuiApplication
{
public:
    QWarApplication(int argc, char *argv[]);

public slots:
    void onGameQuit(void);
private:
    std::unique_ptr<QQmlApplicationEngine>  _qmlAppEngine;
    QGameController                         _gameController;
    QGameModel                              _gameModel;
};

