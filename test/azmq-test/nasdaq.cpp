#include <azmq/socket.hpp>
#include <boost/asio/io_context.hpp>
#include <array>
#include <iostream>

namespace asio = boost::asio;
using namespace std;

int main(int argc, char** argv)
{
    asio::io_service ios;

    azmq::pub_socket publisher(ios);
    azmq::sub_socket subscriber(ios);

    publisher.bind("inproc://nasdaq-feed");

    subscriber.connect("inproc://nasdaq-feed");
    subscriber.set_option(azmq::socket::subscribe("NASDAQ"));

    auto publish = [&]() {
        for (int n = 0;; n++)
        {
            std::string text = "NASDAQ" + std::to_string(n % 10);
            publisher.send(asio::buffer(text));
            cout << "sending " << text << "\n" << flush;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    thread publishThread(publish);

    std::array<char, 256> rcvbuf;

    for (;;) {
        auto size = subscriber.receive(asio::buffer(rcvbuf));
        std::cout << "rcvd: " << std::string(rcvbuf.data(), size) << "\n" << std::flush;
    }
    return 0;
}
