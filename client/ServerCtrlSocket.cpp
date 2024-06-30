#include "ServerCtrlSocket.h"
#include <iostream>
#include <string>

const std::string LOCAL_SERVER_CTRL_URL= "inproc://server_ctrl";

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

void ServerCtrlSocket::Connect(void)
{
    socket_.connect(LOCAL_SERVER_CTRL_URL);
}

