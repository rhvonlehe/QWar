#pragma once

#include <azmq/socket.hpp>

namespace ba = boost::asio;

class GameplayCommunicator
{
public:
    GameplayCommunicator(ba::io_context ctx);



private:
    azmq::router_socket gameplayRouter_;

};
