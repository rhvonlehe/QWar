#pragma once

#include <azmq/socket.hpp>

namespace ba = boost::asio;

class StateReceiver
{
public:
    StateReceiver(ba::io_context& ctx);

    void ReceiveHandler(azmq::message& msg, size_t bytes_transferred);

private:
    azmq::pull_socket   pullSocket_;
};
