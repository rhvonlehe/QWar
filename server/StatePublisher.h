#pragma once

#include <azmq/socket.hpp>

namespace ba = boost::asio;

class StatePublisher
{
public:
    StatePublisher(ba::io_context& ctx);


private:
    azmq::pub_socket    pubSocket_;
};
