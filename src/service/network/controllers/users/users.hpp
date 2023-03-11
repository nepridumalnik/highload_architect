#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <boost/beast.hpp>

#include <memory>
#include <vector>

class UsersTable;
class UsersAuthTable;

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
    bool HandleRequest(const std::string &route,
                       const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                       boost::beast::websocket::response_type &res) final;

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Таблица с пользователями
    std::shared_ptr<UsersAuthTable> authTable_;

    /// @brief Контроллер регистрации пользователей
    std::vector<std::unique_ptr<AbstractController>> subcontrollers_;
};
