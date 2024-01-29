#include "ServerCtrlSocketAdapter.h"

ServerCtrlSocketAdapter::ServerCtrlSocketAdapter(QObject *parent)
    : QObject{parent}
{}

void ServerCtrlSocketAdapter::connectLocal(void)
{
    socket_.StartAndConnect();
}
