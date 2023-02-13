#include <azmq/socket.hpp>
#include <boost/asio.hpp>
#include <array>
#include <iostream>

namespace asio = boost::asio;

int main(int argc, char** argv)
{
    std::cout << "here 0\n";
    asio::io_service ios;
    azmq::sub_socket subSocket(ios);
    subSocket.connect("tcp://127.0.0.1:10000");
    subSocket.set_option(azmq::socket::subscribe("TESTMESSAGE"));

    std::array<char, 256 * 1000> buf;

    for (;;) {
        auto size = subSocket.receive(asio::buffer(buf));
        std::cout << std::string(buf.data(), size) << "\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
