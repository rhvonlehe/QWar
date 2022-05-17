#pragma once

#include <memory>
#include <system_error>
#include <chrono>
#include <iostream>
#define ASIO_STANDALONE
#include <boost/asio.hpp>

namespace ba = boost::asio;

namespace net
{
template<typename T>
class server_interface;

template<typename T>
class connection : public std::enable_shared_from_this<connection<T>>
{
public:
    // A connection is "owned" by either a server or a client, and its
    // behaviour is slightly different bewteen the two.
    enum class owner
    {
        server,
        client
    };

public:
    // Constructor: Specify Owner, connect to context, transfer the socket
    //				Provide reference to incoming message queue
    connection(owner parent, ba::io_context& asioContext, ba::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn)
        : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn)
    {
        m_nOwnerType = parent;

        // Construct validation check data
        if (m_nOwnerType == owner::server)
        {
            // Connection is Server -> Client, construct random data for the client
            // to transform and send back for validation
            _handshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());

            // Pre-calculate the result for checking when the client responds
            _handshakeCheck = scramble(_handshakeOut);
        }
        else
        {
            // Connection is Client -> Server, so we have nothing to define,
            _handshakeIn = 0;
            _handshakeOut = 0;
        }
    }

    virtual ~connection()
    {}

    // This ID is used system wide - its how clients will understand other clients
    // exist across the whole system.
    uint32_t GetID() const
    {
        return id;
    }

public:
    void ConnectToClient(net::server_interface<T>* server, uint32_t uid = 0)
    {
        if (m_nOwnerType == owner::server)
        {
            if (m_socket.is_open())
            {
                id = uid;

                // Was: ReadHeader();

                // A client has attempted to connect to the server, but we wish
                // the client to first validate itself, so first write out the
                // handshake data to be validated
                WriteValidation();

                // Next, issue a task to sit and wait asynchronously for precisely
                // the validation data sent back from the client
                ReadValidation(server);
            }
        }
    }

    void ConnectToServer(const ba::ip::tcp::resolver::results_type& endpoints)
    {
        // Only clients can connect to servers
        if (m_nOwnerType == owner::client)
        {
            // Request asio attempts to connect to an endpoint
            ba::async_connect(m_socket, endpoints,
                                [this](std::error_code ec, ba::ip::tcp::endpoint endpoint)
            {
                if (!ec)
                {
                    // Was: ReadHeader();

                    // First thing server will do is send packet to be validated
                    // so wait for that and respond
                    ReadValidation();
                }
            });
        }
    }


    void Disconnect()
    {
        if (IsConnected())
            ba::post(m_asioContext, [this]() { m_socket.close(); });
    }

    bool IsConnected() const
    {
        return m_socket.is_open();
    }

    // Prime the connection to wait for incoming messages
    void StartListening()
    {

    }

public:
    // ASYNC - Send a message, connections are one-to-one so no need to specifiy
    // the target, for a client, the target is the server and vice versa
    void Send(const message<T>& msg)
    {
        ba::post(m_asioContext,
                   [this, msg]()
        {
            // If the queue has a message in it, then we must
            // assume that it is in the process of asynchronously being written.
            // Either way add the message to the queue to be output. If no messages
            // were available to be written, then start the process of writing the
            // message at the front of the queue.
            bool bWritingMessage = !m_qMessagesOut.empty();
            m_qMessagesOut.push_back(msg);
            if (!bWritingMessage)
            {
                WriteHeader();
            }
        });
    }



private:
    // ASYNC - Prime context to write a message header
    void WriteHeader()
    {
        // If this function is called, we know the outgoing message queue must have
        // at least one message to send. So allocate a transmission buffer to hold
        // the message, and issue the work - asio, send these bytes
        ba::async_write(m_socket, ba::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
                          [this](std::error_code ec, std::size_t length)
        {
            // asio has now sent the bytes - if there was a problem
            // an error would be available...
            if (!ec)
            {
                // ... no error, so check if the message header just sent also
                // has a message body...
                if (m_qMessagesOut.front().body.size() > 0)
                {
                    // ...it does, so issue the task to write the body bytes
                    WriteBody();
                }
                else
                {
                    // ...it didnt, so we are done with this message. Remove it from
                    // the outgoing message queue
                    m_qMessagesOut.pop_front();

                    // If the queue is not empty, there are more messages to send, so
                    // make this happen by issuing the task to send the next header.
                    if (!m_qMessagesOut.empty())
                    {
                        WriteHeader();
                    }
                }
            }
            else
            {
                // ...asio failed to write the message, we could analyse why but
                // for now simply assume the connection has died by closing the
                // socket. When a future attempt to write to this client fails due
                // to the closed socket, it will be tidied up.
                std::cout << "[" << id << "] Write Header Fail.\n";
                m_socket.close();
            }
        });
    }

    // ASYNC - Prime context to write a message body
    void WriteBody()
    {
        // If this function is called, a header has just been sent, and that header
        // indicated a body existed for this message. Fill a transmission buffer
        // with the body data, and send it!
        ba::async_write(m_socket, ba::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
                          [this](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                // Sending was successful, so we are done with the message
                // and remove it from the queue
                m_qMessagesOut.pop_front();

                // If the queue still has messages in it, then issue the task to
                // send the next messages' header.
                if (!m_qMessagesOut.empty())
                {
                    WriteHeader();
                }
            }
            else
            {
                // Sending failed, see WriteHeader() equivalent for description :P
                std::cout << "[" << id << "] Write Body Fail.\n";
                m_socket.close();
            }
        });
    }

    // ASYNC - Prime context ready to read a message header
    void ReadHeader()
    {
        // If this function is called, we are expecting asio to wait until it receives
        // enough bytes to form a header of a message. We know the headers are a fixed
        // size, so allocate a transmission buffer large enough to store it. In fact,
        // we will construct the message in a "temporary" message object as it's
        // convenient to work with.
        ba::async_read(m_socket, ba::buffer(&_msgTemporaryIn.header, sizeof(message_header<T>)),
                         [this](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                // A complete message header has been read, check if this message
                // has a body to follow...
                if (_msgTemporaryIn.header.size > 0)
                {
                    // ...it does, so allocate enough space in the messages' body
                    // vector, and issue asio with the task to read the body.
                    _msgTemporaryIn.body.resize(_msgTemporaryIn.header.size);
                    ReadBody();
                }
                else
                {
                    // it doesn't, so add this bodyless message to the connections
                    // incoming message queue
                    AddToIncomingMessageQueue();
                }
            }
            else
            {
                // Reading form the client went wrong, most likely a disconnect
                // has occurred. Close the socket and let the system tidy it up later.
                std::cout << "[" << id << "] Read Header Fail.\n";
                m_socket.close();
            }
        });
    }

    // ASYNC - Prime context ready to read a message body
    void ReadBody()
    {
        // If this function is called, a header has already been read, and that header
        // request we read a body, The space for that body has already been allocated
        // in the temporary message object, so just wait for the bytes to arrive...
        ba::async_read(m_socket, ba::buffer(_msgTemporaryIn.body.data(), _msgTemporaryIn.body.size()),
                         [this](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                // ...and they have! The message is now complete, so add
                // the whole message to incoming queue
                AddToIncomingMessageQueue();
            }
            else
            {
                // As above!
                std::cout << "[" << id << "] Read Body Fail.\n";
                m_socket.close();
            }
        });
    }

    // "Encrypt" data
    uint64_t scramble(uint64_t nInput)
    {
        uint64_t out = nInput ^ 0xDEADBEEFC0DECAFE;
        out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
        return out ^ 0xC0DEFACE12345678;
    }

    // ASYNC - Used by both client and server to write validation packet
    void WriteValidation()
    {
        ba::async_write(m_socket, ba::buffer(&_handshakeOut, sizeof(uint64_t)),
                          [this](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                // Validation data sent, clients should sit and wait
                // for a response (or a closure)
                if (m_nOwnerType == owner::client)
                    ReadHeader();
            }
            else
            {
                m_socket.close();
            }
        });
    }

    void ReadValidation(net::server_interface<T>* server = nullptr)
    {
        ba::async_read(m_socket, ba::buffer(&_handshakeIn, sizeof(uint64_t)),
                         [this, server](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                if (m_nOwnerType == owner::server)
                {
                    // Connection is a server, so check response from client

                    // Compare sent data to actual solution
                    if (_handshakeIn == _handshakeCheck)
                    {
                        // Client has provided valid solution, so allow it to connect properly
                        std::cout << "Client Validated" << std::endl;
                        server->OnClientValidated(this->shared_from_this());

                        // Sit waiting to receive data now
                        ReadHeader();
                    }
                    else
                    {
                        // Client gave incorrect data, so disconnect
                        std::cout << "Client Disconnected (Fail Validation)" << std::endl;
                        m_socket.close();
                    }
                }
                else
                {
                    // Connection is a client, so solve puzzle
                    _handshakeOut = scramble(_handshakeIn);

                    // Write the result
                    WriteValidation();
                }
            }
            else
            {
                // Some biggerfailure occured
                std::cout << "Client Disconnected (ReadValidation)" << std::endl;
                m_socket.close();
            }
        });
    }

    // Once a full message is received, add it to the incoming queue
    void AddToIncomingMessageQueue()
    {
        // Shove it in queue, converting it to an "owned message", by initialising
        // with the a shared pointer from this connection object
        if(m_nOwnerType == owner::server)
            m_qMessagesIn.push_back({ this->shared_from_this(), _msgTemporaryIn });
        else
            m_qMessagesIn.push_back({ nullptr, _msgTemporaryIn });

        // We must now prime the asio context to receive the next message. It
        // wil just sit and wait for bytes to arrive, and the message construction
        // process repeats itself. Clever huh?
        ReadHeader();
    }

protected:
    // Each connection has a unique socket to a remote
    ba::ip::tcp::socket m_socket;

    // This context is shared with the whole asio instance
    ba::io_context& m_asioContext;

    // This queue holds all messages to be sent to the remote side
    // of this connection
    tsqueue<message<T>> m_qMessagesOut;

    // This references the incoming queue of the parent object
    tsqueue<owned_message<T>>& m_qMessagesIn;

    // Incoming messages are constructed asynchronously, so we will
    // store the part assembled message here, until it is ready
    message<T> _msgTemporaryIn;

    // The "owner" decides how some of the connection behaves
    owner m_nOwnerType = owner::server;

    // Handshake Validation
    uint64_t _handshakeOut = 0;
    uint64_t _handshakeIn = 0;
    uint64_t _handshakeCheck = 0;

    bool _validHandshake = false;
    bool _connectionEstablished = false;

    uint32_t id = 0;
};
}
