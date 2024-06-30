#include "ServerPlaySocket.h"

const std::string DEFAULT_SERVER_PLAY_URL= "tcp://127.0.0.1:11998";

ServerPlaySocket::ServerPlaySocket()
    : socket_(ctx_)
{
}

void ServerPlaySocket::Connect(void)
{
    socket_.connect(DEFAULT_SERVER_PLAY_URL);
}

void ServerPlaySocket::Send(void)
{
    socket_.send(ba::buffer("This is the client's message to the server"));
    azmq::message msg;
    socket_.send(msg);
}
