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
    /// @param usersTable Таблица пользователей
    UserLoginController(std::shared_ptr<UsersTable> usersTable);

    /// @see AbstractController
    bool HandleRequest(const std::string &route,
                       const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                       boost::beast::websocket::response_type &res) final;

private:
    /// @brief Вход в учётную запись
    /// @param req Запрос
    /// @param res Ответ
    void login(const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
               boost::beast::websocket::response_type &res);

    /// @brief Выход из учётной записи
    /// @param req Запрос
    /// @param res Ответ
    void unauthorize(const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                     boost::beast::websocket::response_type &res);

    /// @brief Подтверждение авторизации
    /// @param req Запрос
    /// @param res Ответ
    void authenticate(const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                      boost::beast::websocket::response_type &res);

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Путь до /user/register
    static const std::string route_;
};
