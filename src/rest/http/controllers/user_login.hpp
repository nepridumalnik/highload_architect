#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <memory>

class UsersTable;
class UsersAuthTable;

/// @brief Контроллер авторизации пользователей с путём /login
class UserLoginController : public Poco::Net::HTTPRequestHandler
{
public:
    /// @brief Конструктор
    /// @param usersTable Таблица пользователей
    /// @param authTable Таблица авторизаций
    UserLoginController(std::shared_ptr<UsersTable> usersTable,
                        std::shared_ptr<UsersAuthTable> authTable);

    /// @see HTTPRequestHandler
    void handleRequest(Poco::Net::HTTPServerRequest &req,
                       Poco::Net::HTTPServerResponse &res) final;

private:
    /// @brief Вход в учётную запись
    /// @param req Запрос
    /// @param res Ответ
    void login(Poco::Net::HTTPServerRequest &req,
               Poco::Net::HTTPServerResponse &res);

    /// @brief Выход из учётной записи
    /// @param req Запрос
    /// @param res Ответ
    void unauthorize(Poco::Net::HTTPServerRequest &req,
                     Poco::Net::HTTPServerResponse &res);

    /// @brief Подтверждение авторизации
    /// @param req Запрос
    /// @param res Ответ
    void authenticate(Poco::Net::HTTPServerRequest &req,
                      Poco::Net::HTTPServerResponse &res);

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Таблица авторизаций
    std::shared_ptr<UsersAuthTable> authTable_;
};
