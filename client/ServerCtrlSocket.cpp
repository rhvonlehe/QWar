#include "ServerCtrlSocket.h"

ServerCtrlSocket::ServerCtrlSocket(void)
    : socket_(ios_)
{
}

void ServerCtrlSocket::StartAndConnect(void)
{
    socket_.connect(SERVER_SOCKET_URL);
}
