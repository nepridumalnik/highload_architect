#include <service/network/connection.hpp>

#include <iostream>
#include <thread>

Connection::Connection(const uint16_t port, callback cb)
    : context_{}, endpoint_{boost::asio::ip::tcp::v4(), port}, acceptor_{context_, endpoint_}, cb_{cb}
{
}

void Connection::operator()()
{
    do
    {
        boost::asio::ip::tcp::socket socket{context_};
        acceptor_.accept(socket);
        std::thread(&Connection::runTask, this, std::move(socket)).detach();
    } while (true);
}

void Connection::runTask(boost::asio::ip::tcp::socket &&socket)
{
    try
    {
        cb_(socket);
        socket.close();
    }
    catch (...)
    {
    }
}
