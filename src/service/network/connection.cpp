#include <service/network/connection.hpp>

#include <boost/thread.hpp>

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

            boost::thread socket_thread(
                [cb = cb_, socket = std::move(socket)]() mutable
                {
                    try
                    {
                        cb(std::move(socket));
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                    catch (...)
                    {
                        std::cerr << "Unknown error" << std::endl;
                    }
                });
            socket_thread.detach();
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
