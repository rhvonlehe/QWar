#include "ServerCtrlSocketAdapter.h"

ServerCtrlSocketAdapter::ServerCtrlSocketAdapter(QObject *parent)
    : QObject{parent}
{}

void ServerCtrlSocketAdapter::startAndConnectLocal(void)
{
    socket_.StartLocalServer();
    socket_.ConnectLocalServer();
}

void ServerCtrlSocketAdapter::connectRemote(void)
{
    socket_.ConnectRemoteServer();
}
