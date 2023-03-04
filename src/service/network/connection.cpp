#include <service/network/connection.hpp>

#include <iostream>

Connection::Connection(const size_t port, callback cb)
    : context_{}, endpoint_{boost::asio::ip::tcp::v4(), port}, acceptor_{context_, endpoint_}, cb_{cb}
{
}

void Connection::operator()()
{
    do
    {
        try
        {
            boost::asio::ip::tcp::socket socket(context_);
            acceptor_.accept(socket);
            cb_(std::move(socket));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown error" << std::endl;
        }
    } while (true);
}
