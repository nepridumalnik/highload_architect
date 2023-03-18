#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <memory>

class UsersTable;

/// @brief Контроллер получения списка пользователей с путём /user/search
class UserSearchController : public AbstractController
{
public:
    /// @brief Конструктор
    explicit UserSearchController(std::shared_ptr<UsersTable> usersTable);

    /// @see AbstractController
    bool HandleRequest(const std::string &route,
                       const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                       boost::beast::websocket::response_type &res) final;

private:
    /// @brief Найти пользователей
    /// @param req Запрос
    /// @param res Ответ
    void searchUsers(const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                     boost::beast::websocket::response_type &res);

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Путь до /user/search
    static const std::string route_;
};
