#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <boost/beast.hpp>

#include <memory>

class UsersTable;

/// @brief Контроллер авторизации пользователей с путём /login
class UserLoginController : public AbstractController
{
public:
    /// @brief Конструктор
    UserLoginController(std::shared_ptr<UsersTable> usersTable);

    /// @see AbstractController
    void HandleRequest(
        const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
        boost::beast::websocket::response_type &res) final;

    /// @see AbstractController
    bool HasRoute(const std::string &route) final;

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Путь до /user/register
    static const std::string route_;
};
