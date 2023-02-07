#include <azmq/socket.hpp>
#include <boost/asio/io_context.hpp>
#include <msgpack.hpp>
#include <iostream>

namespace asio = boost::asio;
using namespace std;

#define PUB_SUB 0

int main()
{
    asio::io_context ioc;
    asio::io_context ioc_client;

#if PUB_SUB  // pub & sub sockets
    azmq::pub_socket client(ioc);
    azmq::sub_socket server(ioc);
    server.set_option(azmq::socket::subscribe("msgpack"));
#else  // router & dealer sockets
    azmq::router_socket server(ioc);
    azmq::dealer_socket client(ioc_client);
    //    azmq::rep_socket server(ioc);
    //    azmq::req_socket client(ioc);
#endif
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

//        while(true)
        {
            //            auto size = server.receive(asio::buffer(buffer));
            server.async_receive(asio::buffer(rcv_bufs),
                                 [&](boost::system::error_code const& ec, size_t bytes_transferred) {
                std::cout << "rcvd size: " << bytes_transferred << "\n" << std::flush;
                msgpack::object_handle oh = msgpack::unpack(static_cast<char*>(rcv_bufs[1].data()), bytes_transferred);
                msgpack::object deserialized = oh.get();
                std::cout << "three values: " << deserialized << "\n" << std::flush;

                msgpack::type::tuple<int, bool, string> dst;
                deserialized.convert(dst);
            });

            ioc.run();

#if PUB_SUB
            msgpack::object_handle oh = msgpack::unpack(&buffer[7], size - 7);
#else
            //            msgpack::object_handle oh = msgpack::unpack(buffer.data(), size);
#endif
            //            msgpack::object deserialized = oh.get();
            //            std::cout << "three values: " << deserialized << "\n" << std::flush;

            //            msgpack::type::tuple<int, bool, string> dst;
            //            deserialized.convert(dst);
            //            break; // TEMP
        }
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
#if PUB_SUB
            client.send(asio::buffer(text));
#else
            client.send(asio::buffer(ssbuffer.str()));  //,
//                              [&](boost::system::error_code const& ec, size_t bytes_transferred) {});
#endif
            std::this_thread::sleep_for(std::chrono::seconds(1));
            //            break; // TEMP
        }
    };

    thread serverThread(runServer);
    thread clientThread(runClient);

    ioc.run();
    ioc_client.run();

    while(1) {}
}
