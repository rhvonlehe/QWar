#include "StatePushSocket.h"

const std::string DEFAULT_SERVER_PULL_URL= "tcp://127.0.0.1:11999";

StatePushSocket::StatePushSocket()
    : socket_(ctx_)
{
}

void StatePushSocket::Connect(void)
{
    socket_.connect(DEFAULT_SERVER_PULL_URL);
}

void StatePushSocket::Send(void)
{
    socket_.send(azmq::message("This is a pushed state msg"));
    socket_.send(azmq::message("This is the client's push message to the server"));
    // azmq::message msg;
    // socket_.send(msg);
}
