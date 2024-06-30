#pragma once

#include <ServerCtrlSocket.h>
#include <QObject>

class ServerCtrlSocketAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ServerCtrlSocketAdapter(QObject *parent = nullptr);

signals:

private:
    ServerCtrlSocket socket_;
};
