#include "Server.h"
#include <iostream>

using namespace std;

Server::Server(void) :
    control_(ctx_),
    gameplay_(ctx_)
{}


void Server::operator()(void)
{
    cout << "server started" << endl;
    control_.bind("inproc://server_ctrl");
    gameplay_.bind("tcp://*:11998");

    cout << "Waiting on server play message" << endl;
    gameplay_.async_receive([](boost::system::error_code ec, azmq::message& msg, size_t bytes_transferred) {
        cout << "gameplay ctrl socket received message: " << endl;
    });

    cout << "Waiting on server ctrl messages" << endl;
    control_.async_receive([]( boost::system::error_code ec, azmq::message& msg, size_t bytes_transferred) {
        cout << "server ctrl socket received message: " << endl;
    });


    ctx_.run();

    while(1)
    {

    }
}


