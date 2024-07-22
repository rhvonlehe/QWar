#pragma once

// #include <ServerCtrlSocket.h> // TODO remove
#include <StatePushSocket.h>
#include <Server.h>
#include <thread>
#include <QObject>

class ClientAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ClientAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void startAndConnectLocal(void);
    Q_INVOKABLE void connectRemote(void);

    Q_INVOKABLE void testSend(void);

signals:

private:
    // ServerCtrlSocket ctrlSocket_; // TODO remove
    std::unique_ptr<Server>     server_;
    std::thread                 serverThread_;
    StatePushSocket             pushSocket_;
};
