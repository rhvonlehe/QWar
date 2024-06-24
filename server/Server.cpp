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


}


