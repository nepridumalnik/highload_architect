#include <service/network/connection.hpp>

#include <iostream>
#include <future>

Connection::Connection(const size_t port, callback cb)
    : context_{}, endpoint_{boost::asio::ip::tcp::v4(), port}, acceptor_{context_, endpoint_}, cb_{cb}
{
}

void Connection::operator()()
{
    do
    {
        boost::asio::ip::tcp::socket socket(context_);
        acceptor_.accept(socket);

        const auto _ = std::async(std::launch::async,
                                  [cb = cb_, socket = std::move(socket)]() mutable
                                  {
                                      try
                                      {
                                          cb(socket);
                                          socket.close();
                                      }
                                      catch (...)
                                      {
                                      }
                                  });
    } while (true);
}
