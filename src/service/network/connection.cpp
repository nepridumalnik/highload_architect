#include <service/network/connection.hpp>

#include <boost/bind.hpp>

#include <iostream>
#include <thread>
#include <memory>

Connection::Connection(const uint16_t port, callback cb)
    : context_{}, endpoint_{boost::asio::ip::tcp::v4(), port}, acceptor_{context_, endpoint_}, cb_{cb}
{
}

void Connection::Run()
{
    std::thread t{[this]() -> void
                  {
                      boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work =
                          boost::asio::make_work_guard(context_);
                      context_.run();
                  }};

    do
    {
        // Сделано чтобы не копировать/перемещать сокет
        std::shared_ptr<boost::asio::ip::tcp::socket> socket =
            std::make_shared<boost::asio::ip::tcp::socket>(context_);
        acceptor_.accept(*socket);

        boost::asio::post(context_, boost::bind(&Connection::runTask, this, std::move(socket)));
    } while (true);

    t.join();
}

void Connection::runTask(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    try
    {
        cb_(*socket);
        socket->close();
    }
    catch (...)
    {
    }
}
