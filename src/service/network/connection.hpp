#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <functional>
#include <memory>

/// @brief Колбэк, вызываемый с сокетом
using callback = std::function<void(boost::asio::ip::tcp::socket &)>;

/// @brief Соединение
class Connection
{
public:
    /// @brief Конструктор
    Connection(const size_t port, callback cb);

    /// @brief Получение соединений
    void operator()();

private:
    /// @brief Запуск задачи
    /// @param socket Сокет
    void runTask(boost::asio::ip::tcp::socket socket);

private:
    /// @brief Контекст asio
    boost::asio::io_context context_;

    /// @brief Точка соединения
    boost::asio::ip::tcp::endpoint endpoint_;

    /// @brief Приёмник соединений
    boost::asio::ip::tcp::acceptor acceptor_;

    /// @brief Колбэк, вызываемый с сокетом
    callback cb_;
};
