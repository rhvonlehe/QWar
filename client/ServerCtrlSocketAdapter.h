#pragma once

#include <ServerCtrlSocket.h>
#include <QObject>

class ServerCtrlSocketAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ServerCtrlSocketAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void startAndConnectLocal(void);
    Q_INVOKABLE void connectRemote(void);
signals:

private:
    ServerCtrlSocket socket_;
};
