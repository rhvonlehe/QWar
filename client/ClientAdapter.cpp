#include "ClientAdapter.h"

ClientAdapter::ClientAdapter(QObject *parent)
    : QObject{parent}
{}

void ClientAdapter::startAndConnectLocal(void)
{
    ctrlSocket_.StartLocalServer();


    ctrlSocket_.Connect();
    playSocket_.Connect();

}

void ClientAdapter::connectRemote(void)
{
    playSocket_.Connect();
}

void ClientAdapter::testSend(void)
{
    playSocket_.Send();
}
