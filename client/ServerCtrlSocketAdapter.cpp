#include "ServerCtrlSocketAdapter.h"

ServerCtrlSocketAdapter::ServerCtrlSocketAdapter(QObject *parent)
    : QObject{parent}
{}

void ServerCtrlSocketAdapter::Connect(void)
{
    socket_.StartAndConnect();
}
