#include "ClientAdapter.h"
#include <iostream>

using namespace std;

ClientAdapter::ClientAdapter(QObject *parent)
    : QObject{parent}
{}

void ClientAdapter::startAndConnectLocal(void)
{
    server_ = make_unique<Server>();
    serverThread_ = std::thread([&]() {
        cout <<"starting server thread" << endl;
        server_->operator()();
    });



    pushSocket_.Connect();

}

void ClientAdapter::connectRemote(void)
{
    pushSocket_.Connect();
}

void ClientAdapter::testSend(void)
{
    pushSocket_.Send();
}
