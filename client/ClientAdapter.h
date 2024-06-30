#pragma once

#include <ServerCtrlSocket.h>
#include <ServerPlaySocket.h>
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
    ServerCtrlSocket ctrlSocket_;
    ServerPlaySocket playSocket_;
};
