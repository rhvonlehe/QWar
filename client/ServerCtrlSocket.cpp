#include "ServerCtrlSocket.h"
#include <iostream>


ServerCtrlSocket::ServerCtrlSocket(void)
    : socket_(ctx_)
{
}

void ServerCtrlSocket::StartAndConnect(void)
{
    serverThread_ = std::thread([&]() {
        std::cout << "starting server thread" << std::endl;
        server_();
    });

    socket_.connect(SERVER_SOCKET_URL);
}
