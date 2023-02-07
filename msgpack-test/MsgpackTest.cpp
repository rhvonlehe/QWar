#include <azmq/socket.hpp>
#include <boost/asio/io_context.hpp>
#include <msgpack.hpp>
#include <iostream>

namespace asio = boost::asio;
using namespace std;

#define PUB_SUB 0

int main()
{
    asio::io_context ioc_server;
    asio::io_context ioc_client;

    azmq::router_socket server(ioc_server);
    azmq::dealer_socket client(ioc_client);

    server.connect("inproc://server");
    client.bind("inproc://server");

    auto runServer = [&]()
    {
        std::array<char, 100> ident;
        std::array<char, 5000> buffer;
        std::array<asio::mutable_buffer, 2> rcv_bufs =
        {{
             asio::buffer(ident),
             asio::buffer(buffer)
         }};

        server.async_receive(rcv_bufs,
                             [&](boost::system::error_code const& ec, size_t bytes_transferred) {
            std::cout << "rcvd size: " << bytes_transferred << "\n" << std::flush;
            msgpack::object_handle oh = msgpack::unpack(static_cast<char*>(rcv_bufs[1].data()), bytes_transferred);
            msgpack::object deserialized = oh.get();
            std::cout << "three values: " << deserialized << "\n" << std::flush;

            msgpack::type::tuple<int, bool, string> dst;
            deserialized.convert(dst);
//            ioc_server.stop();
        });

        ioc_server.run();
    };

    auto runClient = [&]()
    {
        msgpack::type::tuple<int, bool, std::string> src(1, true, "example");
        std::stringstream ssbuffer;
        msgpack::pack(ssbuffer, src);

        while(true)
        {
            std::string text = "msgpack" + ssbuffer.str();
            cout << "sending " << text << "\n" << flush;
            client.send(asio::buffer(ssbuffer.str()));  //,
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    thread serverThread(runServer);
    thread clientThread(runClient);

//    ioc_server.run();
//    ioc_client.run();

    while(1) {}
}
