#pragma once

#include <azmq/socket.hpp>
#include <boost/asio.hpp>

namespace ba = boost::asio;

class StatePushSocket
{
public:
    StatePushSocket();

    void Connect(void);
    void Send(void);  // TODO

private:
    ba::io_context          ctx_;
    azmq::push_socket       socket_;
};
