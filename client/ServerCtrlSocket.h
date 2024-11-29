#pragma once

#include <Server.h>
#include <azmq/socket.hpp>
#include <boost/asio.hpp>
#include <thread>

namespace ba = boost::asio;


class ServerCtrlSocket
{
public:
    ServerCtrlSocket(void);

    void StartLocalServer(void);
    void Connect(void);

    // void Send(void);

    // void RegisterCB();


private:
    std::thread             serverThread_;
    std::unique_ptr<Server> server_;
    ba::io_context          ctx_;
    azmq::dealer_socket     socket_;
};
