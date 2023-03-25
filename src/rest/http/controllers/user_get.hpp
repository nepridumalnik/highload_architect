#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <memory>

class UsersTable;

/// @brief Контроллер получения пользователей
class UserGetController : public Poco::Net::HTTPRequestHandler
{
public:
    /// @brief Конструктор
    explicit UserGetController(std::shared_ptr<UsersTable> usersTable);

    /// @see HTTPRequestHandler
    void handleRequest(Poco::Net::HTTPServerRequest &req,
                       Poco::Net::HTTPServerResponse &res) final;

private:
    /// @brief Получить пользователя по идентификатору
    /// @param id Идентификатор
    /// @param req Запрос
    /// @param res Ответ
    void getUserById(const size_t id,
                     Poco::Net::HTTPServerRequest &req,
                     Poco::Net::HTTPServerResponse &res);

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;
};
