#pragma once

#include <azmq/socket.hpp>
#include <boost/asio.hpp>

namespace ba = boost::asio;

class ServerPlaySocket
{
public:
    ServerPlaySocket();

    void Connect(void);
    void Send(void);  // TODO

private:
    ba::io_context          ctx_;
    azmq::dealer_socket     socket_;
};
