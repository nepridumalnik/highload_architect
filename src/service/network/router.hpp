#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <boost/asio.hpp>

#include <memory>
#include <vector>

/// @brief Класс для маршрутизации
class Router
{
public:
    /// @brief Обработать входящий пакет
    /// @param socket Сокет
    void operator()(boost::asio::ip::tcp::socket &socket);

    /// @brief Зарегистрировать контроллер
    /// @param controller Контроллер
    void AddController(std::unique_ptr<AbstractController> controller);

private:
    /// @brief Контроллеры
    std::vector<std::unique_ptr<AbstractController>> controllers_;
};
