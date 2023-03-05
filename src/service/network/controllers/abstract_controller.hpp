#pragma once

#include <boost/beast.hpp>

/// @brief Интерфейс контроллера
struct AbstractController
{
    /// @brief Деструктор
    virtual ~AbstractController() = default;

    /// @brief Обработать соединение
    /// @param req Запрос
    /// @param res Ответ
    virtual void HandleRequest(
        const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
        boost::beast::websocket::response_type &res) = 0;
};
