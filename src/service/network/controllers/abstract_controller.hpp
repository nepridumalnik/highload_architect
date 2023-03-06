#pragma once

#include <boost/beast.hpp>

/// @brief Интерфейс обработчика
struct AbstractHttpHandler
{
    /// @brief Деструктор
    virtual ~AbstractHttpHandler() = default;

    /// @brief Обработать соединение
    /// @param req Запрос
    /// @param res Ответ
    virtual void HandleRequest(
        const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
        boost::beast::websocket::response_type &res) = 0;
};

/// @brief Интерфейс пути
struct AbstractRoute
{
    /// @brief Деструктор
    virtual ~AbstractRoute() = default;

    /// @brief Проверить, обрабатываем ли маршрут
    /// @param route Маршрут
    /// @return True, если обрабатываем
    virtual bool HasRoute(const std::string &route) = 0;
};

/// @brief Интерфейс контроллера
struct AbstractController : public AbstractHttpHandler, public AbstractRoute
{
};
