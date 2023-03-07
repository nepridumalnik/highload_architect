#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <boost/beast.hpp>

#include <memory>

class UsersTable;

/// @brief Контроллер регистрации пользователей
class UserRegisterController : public AbstractHttpHandler
{
public:
    /// @brief Конструктор
    UserRegisterController(std::shared_ptr<UsersTable> usersTable);

    /// @see AbstractHttpHandler
    void HandleRequest(
        const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
        boost::beast::websocket::response_type &res) final;

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;
};
