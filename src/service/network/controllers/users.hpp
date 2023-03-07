#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <service/database/models/users.hpp>

#include <boost/beast.hpp>

#include <memory>

namespace soci
{
    class session;
} // namespace soci

/// @brief Контроллер пользователей
class UsersController : public AbstractController
{
public:
    /// @brief Конструктор
    UsersController(std::shared_ptr<soci::session> sql);

    /// @see AbstractController
    void HandleRequest(const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                       boost::beast::websocket::response_type &res) final;

    /// @see AbstractController
    bool HasRoute(const std::string &route) final;

private:
    /// @brief Таблица с пользователями
    std::unique_ptr<UsersTable> usersTable_;

    /// @brief Путь до /login
    static const std::string loginRoute;

    /// @brief Путь до /user/register
    static const std::string userRegisterRoute;

    /// @brief Путь до /user/get/:id
    static const std::string userGetIdRoute;
};
