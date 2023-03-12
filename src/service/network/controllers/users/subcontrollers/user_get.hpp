#pragma once

#include <service/network/controllers/abstract_controller.hpp>

#include <boost/beast.hpp>

#include <memory>

class UsersTable;

/// @brief Контроллер получения пользователей с путём /user/get/:id
class UserGetController : public AbstractController
{
public:
    /// @brief Конструктор
    UserGetController(std::shared_ptr<UsersTable> usersTable);

    /// @see AbstractController
    bool HandleRequest(const std::string &route,
                       const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                       boost::beast::websocket::response_type &res) final;

private:
    /// @brief Получить пользователя по идентификатору
    /// @param id Идентификатор
    /// @param req Запрос
    /// @param res Ответ
    void getUserById(const size_t id,
                     const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                     boost::beast::websocket::response_type &res);

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Путь до /user/register
    static const std::string route_;
};
