#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <memory>

class UsersTable;

/// @brief Контроллер регистрации пользователей с путём /user/register
class UserRegisterController : public Poco::Net::HTTPRequestHandler
{
public:
    /// @brief Конструктор
    /// @param usersTable Таблица пользователей
    explicit UserRegisterController(std::shared_ptr<UsersTable> usersTable);

    /// @see HTTPRequestHandler
    void handleRequest(Poco::Net::HTTPServerRequest &req,
                       Poco::Net::HTTPServerResponse &res) final;

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;
};
