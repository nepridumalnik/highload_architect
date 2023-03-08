#pragma once

#include <boost/beast.hpp>

#include <string>

/// @brief Интерфейс контроллера
struct AbstractController
{
    /// @brief Деструктор
    virtual ~AbstractController() = default;

    /// @brief Обработать соединение
    /// @param route Маршрут
    /// @param req Запрос
    /// @param res Ответ
    /// @return True, если запрос обработан
    virtual bool HandleRequest(const std::string &route,
                               const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                               boost::beast::websocket::response_type &res) = 0;
};
