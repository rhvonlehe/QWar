#include "StateReceiver.h"
#include <iostream>

using namespace std;

StateReceiver::StateReceiver(ba::io_context& ctx)
    : pullSocket_(ctx)
{
    pullSocket_.bind("tcp://*:11999");

    pullSocket_.async_receive([this](boost::system::error_code ec, azmq::message& msg, size_t bytes_transferred) {
        this->ReceiveHandler(msg, bytes_transferred);
    });
}

void StateReceiver::ReceiveHandler(azmq::message& msg, size_t bytes_transferred)
{
    cout << "server state PULL socket received a state PUSH" << endl;

    std::array<char, 100> local_buf;
    auto boost_buffer_size = msg.buffer_copy(boost::asio::buffer(local_buf));
    cout << "boost_buffer is: " << endl;
    cout.write(&local_buf[0], boost_buffer_size);
    cout << endl;
    auto more = msg.more();
    cout << "message more is : " << more << endl;
    const char* str = static_cast<const char*>(msg.data());
    auto size = msg.size();
    cout << "msg buffer size: " << size << endl;
    cout << "bytes transferred: " << bytes_transferred << endl;
    cout.write(str, size);
    cout << endl;
    // cout << str << endl;

    pullSocket_.async_receive([this](boost::system::error_code ec, azmq::message& msg, size_t bytes_transferred) {
        this->ReceiveHandler(msg, bytes_transferred);
    });


}
