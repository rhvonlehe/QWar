#include <azmq/socket.hpp>
#include <boost/asio/io_context.hpp>

namespace asio = boost::asio;
using namespace std;

void runServer(void)
{

}


int main()
{
    asio::io_context ioc;

    azmq::dealer_socket dealer(ioc, true);
    azmq::router_socket router(ioc, true);

    router.bind("inproc://server");
    dealer.connect("inproc::server");

    thread serverThread(runServer);


}
