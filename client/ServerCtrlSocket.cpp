#include "ServerCtrlSocket.h"
#include <iostream>
#include <string>

const std::string LOCAL_SERVER_SOCKET_URL= "inproc://localhost:11999";
const std::string DEFAULT_SERVER_URL= "tcp://localhost:11999";

ServerCtrlSocket::ServerCtrlSocket(void)
    : socket_(ctx_)
{
}

void ServerCtrlSocket::StartLocalServer(void)
{
    serverThread_ = std::thread([&]() {
        std::cout << "starting server thread" << std::endl;
        server_();
    });
}

void ServerCtrlSocket::ConnectLocalServer(void)
{
    socket_.connect(LOCAL_SERVER_SOCKET_URL);
}

void ServerCtrlSocket::ConnectRemoteServer(void)
{
    socket_.connect(DEFAULT_SERVER_URL);
}
