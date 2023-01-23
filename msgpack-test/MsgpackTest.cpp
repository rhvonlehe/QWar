#include <azmq/socket.hpp>
#include <boost/asio/io_context.hpp>
#include <msgpack.hpp>
#include <iostream>

namespace asio = boost::asio;
using namespace std;


int main()
{
    asio::io_context ioc;

//    azmq::dealer_socket dealer(ioc, true);
    azmq::pub_socket dealer(ioc);
    dealer.bind("inproc://server");

//    azmq::router_socket router(ioc, true);
    azmq::sub_socket router(ioc);
    router.set_option(azmq::socket::subscribe("msgpack"));
    router.connect("inproc://server");

//    router.bind("inproc://server");
//    dealer.connect("inproc://server");

    msgpack::type::tuple<int, bool, std::string> src(1, true, "example");
    std::stringstream ssbuffer;
    msgpack::pack(ssbuffer, src);

    auto runServer = [&]()
    {
        std::array<char, 5000> buffer;

        while(true)
        {
            auto size = router.receive(asio::buffer(buffer));
            std::cout << "rcvd size: " << size << "\n" << std::flush;

            msgpack::object_handle oh = msgpack::unpack(&buffer[7], size - 7);
            msgpack::object deserialized = oh.get();
            std::cout << "three values: " << deserialized << "\n" << std::flush;

            msgpack::type::tuple<int, bool, string> dst;
            deserialized.convert(dst);
        }
    };

    auto runClient = [&]()
    {
        while(true)
        {
            std::string text = "msgpack" + ssbuffer.str();
//            dealer.send(asio::buffer(ssbuffer.str()));
            dealer.send(asio::buffer(text));
            cout << "sending " << text << "\n" << flush;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    thread serverThread(runServer);
    thread clientThread(runClient);


    while (1) {};
}
