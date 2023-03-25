#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <memory>

class UsersTable;

/// @brief Контроллер получения списка пользователей с путём /user/search
class UserSearchController : public Poco::Net::HTTPRequestHandler
{
public:
    /// @brief Конструктор
    explicit UserSearchController(std::shared_ptr<UsersTable> usersTable);

    /// @see HTTPRequestHandler
    void handleRequest(Poco::Net::HTTPServerRequest &req,
                       Poco::Net::HTTPServerResponse &res) final;

private:
    /// @brief Найти пользователей
    /// @param req Запрос
    /// @param res Ответ
    void searchUsers(Poco::Net::HTTPServerRequest &req,
                     Poco::Net::HTTPServerResponse &res);

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Путь до /user/search
    static const std::string route_;
};
