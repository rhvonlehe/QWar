#include "ServerCtrlSocketAdapter.h"

ServerCtrlSocketAdapter::ServerCtrlSocketAdapter(QObject *parent)
    : QObject{parent}
{}

void ServerCtrlSocketAdapter::startAndConnectLocal(void)
{
}

void ServerCtrlSocketAdapter::connectRemote(void)
{
    socket_.ConnectRemoteServer();
}
