#pragma once

#include "Connection.h"
#include <string>

#define ASIO_STANDALONE
#include <boost/asio.hpp>

namespace net
{
namespace ba = boost::asio;

// Client
template <typename T>
class ClientInterface
{
public:
    ClientInterface()
    {}

    virtual ~ClientInterface()
    {
        // If the client is destroyed, always try and disconnect from server
        Disconnect();
    }

public:
    // Connect to server with hostname/ip-address and port
    bool Connect(const std::string& host, const uint16_t port)
    {
        try
        {
            // Resolve hostname/ip-address into tangiable physical address
            ba::ip::tcp::resolver resolver(_context);
            ba::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

            // Create connection
            _connection = std::make_unique<Connection<T>>(Connection<T>::owner::client,
                                                          _context,
                                                          ba::ip::tcp::socket(_context),
                                                          _qMessagesIn);

            // Tell the connection object to connect to server
            _connection->ConnectToServer(endpoints);

            // Start Context Thread
            _thrContext = std::thread([this]() { _context.run(); });
        }
        catch (std::exception& e)
        {
            std::cerr << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    // Disconnect from server
    void Disconnect()
    {
        // If connection exists, and it's connected then...
        if(IsConnected())
        {
            // ...disconnect from server gracefully
            _connection->Disconnect();
        }

        // Either way, we're also done with the asio context...
        _context.stop();
        // ...and its thread
        if (_thrContext.joinable())
            _thrContext.join();

        // Destroy the connection object
        _connection.release();
    }

    // Check if client is actually connected to a server
    bool IsConnected()
    {
        if (_connection)
            return _connection->IsConnected();
        else
            return false;
    }

public:
    // Send message to server
    void Send(const message<T>& msg)
    {
        if (IsConnected())
            _connection->Send(msg);
    }

    // Retrieve queue of messages from server
    TSQueue<owned_message<T>>& Incoming()
    {
        return _qMessagesIn;
    }

protected:
    // asio context handles the data transfer...
    ba::io_context _context;
    // ...but needs a thread of its own to execute its work commands
    std::thread _thrContext;
    // The client has a single instance of a "connection" object, which handles data transfer
    std::unique_ptr<Connection<T>> _connection;

private:
    // This is the thread safe queue of incoming messages from server
    TSQueue<owned_message<T>> _qMessagesIn;
};

} // namespace net
