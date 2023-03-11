#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <boost/beast.hpp>

#include <memory>

class UsersTable;
class UsersAuthTable;

/// @brief Контроллер регистрации пользователей с путём /user/register
class UserRegisterController : public AbstractController
{
public:
    /// @brief Конструктор
    /// @param usersTable Таблица пользователей
    /// @param authTable Таблица авторизаций
    UserRegisterController(std::shared_ptr<UsersTable> usersTable,
                           std::shared_ptr<UsersAuthTable> authTable);

    /// @see AbstractController
    bool HandleRequest(const std::string &route,
                       const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                       boost::beast::websocket::response_type &res) final;

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Таблица авторизаций
    std::shared_ptr<UsersAuthTable> authTable_;

    /// @brief Путь до /user/register
    static const std::string route_;
};
