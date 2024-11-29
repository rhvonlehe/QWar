#include "ServerCtrlSocket.h"
#include <iostream>
#include <string>

const std::string LOCAL_SERVER_CTRL_URL= "inproc://server_ctrl";
using namespace std;

ServerCtrlSocket::ServerCtrlSocket(void)
    : socket_(ctx_)
{
}

void ServerCtrlSocket::StartLocalServer(void)
{
    server_ = make_unique<Server>();
    serverThread_ = std::thread([&]() {
        std::cout << "starting server thread" << std::endl;
        server_->operator()();
    });
}

void ServerCtrlSocket::Connect(void)
{
    socket_.connect(LOCAL_SERVER_CTRL_URL);
}

