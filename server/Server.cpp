#include "Server.h"
#include <iostream>

using namespace std;

Server::Server(void) :
    publisher_(ctx_),
    stateRcvr_(ctx_)
{}


void Server::GamePlayHandler(azmq::message& msg)
{
    // TODO set up the async_receive to accept 2 messages: the ident and the msg


    cout << "gameplay ctrl socket received message: " << endl;
    cout << msg.buffer().data() << endl;
    // gameplay_.async_receive([&](boost::system::error_code ec, azmq::message& msg, size_t bytes_transferred) {
    //     this->GamePlayHandler(msg);
    // });
}

void Server::operator()(void)
{
    cout << "server started" << endl;

#if 0 // TODO remove
    gameplayComm_ = make_unique<GameplayCommunicator>(ctx_); // TODO remove
    control_.bind("inproc://server_ctrl");
    gameplay_.bind("tcp://*:11998");

    cout << "Waiting on server play message" << endl;
    gameplay_.async_receive([&](boost::system::error_code ec, azmq::message& msg, size_t bytes_transferred) {
        this->GamePlayHandler(msg);
    });

    cout << "Waiting on server ctrl messages" << endl;
    control_.async_receive([]( boost::system::error_code ec, azmq::message& msg, size_t bytes_transferred) {
        cout << "server ctrl socket received message: " << endl;
    });
#endif


    ctx_.run();

    while(1)
    {

    }
}


