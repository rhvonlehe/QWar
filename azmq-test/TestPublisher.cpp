#include <azmq/socket.hpp>
#include <boost/asio.hpp>
#include <array>
#include <iostream>

namespace asio = boost::asio;

int main()
{
    asio::io_service ios;
    azmq::pub_socket pubSocket(ios);
    pubSocket.bind("tcp://127.0.0.1:10000");
    for (int n = 0;; n++) {
        std::string text = "TESTMESSAGE" + std::to_string(n%10);
        std::cout <<text << "\n" << std::flush;
        pubSocket.send(asio::buffer(text));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
