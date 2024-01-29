#pragma once

#include <Server.h>
#include <azmq/socket.hpp>
#include <boost/asio.hpp>
#include <string>
#include <thread>

namespace ba = boost::asio;

const std::string SERVER_SOCKET_URL= "inproc://127.0.0.1:11999";
const std::string SERVER_EXECUTABLE_NAME = "QWarServer";

class ServerCtrlSocket
{
public:
    ServerCtrlSocket(void);

    void StartAndConnect(void);

    void Send(void);

    void RegisterCB();


private:
    std::thread             serverThread_;
    Server                  server_;
    ba::io_context          ctx_;
    azmq::dealer_socket     socket_;
};
