#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <boost/asio.hpp>

#include <memory>
#include <map>

/// @brief Класс для маршрутизации
class Router
{
public:
    /// @brief Обработать входящий пакет
    /// @param socket Сокет
    void operator()(boost::asio::ip::tcp::socket &socket);

    /// @brief Зарегистрировать контроллер
    /// @param route Маршрут
    /// @param controller Контроллер
    void AddController(const std::string &route, std::unique_ptr<AbstractController> controller);

private:
    /// @brief Контроллеры
    std::map<std::string, std::unique_ptr<AbstractController>> controllers_;
};
