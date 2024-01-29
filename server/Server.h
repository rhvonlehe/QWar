
#include <azmq/socket.hpp>

namespace ba = boost::asio;

class Server
{
public:
    Server(void);
    void operator()(void);

private:
    ba::io_context      ctx_;
    azmq::router_socket control_;
    azmq::router_socket gameplay_;
};

