#pragma once

#include <string>
#include <azmq/socket.hpp>
#include <boost/asio.hpp>

const std::string SERVER_SOCKET_URL= "tcp://127.0.0.1:11999";
const std::string SERVER_EXECUTABLE_NAME = "QWarServer";

class ServerCtrlSocket
{
public:
    ServerCtrlSocket(void);

    void StartAndConnect(void);

    void Send(void);

    void RegisterCB();


private:
    boost::asio::io_service ios_;
    azmq::dealer_socket socket_;
};
