#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <memory>

class FriendsTable;

/// @brief Контроллер получения пользователей
class FriendsController : public Poco::Net::HTTPRequestHandler
{
public:
    /// @brief Конструктор
    explicit FriendsController(std::shared_ptr<FriendsTable> friendsTable);

    /// @see HTTPRequestHandler
    void handleRequest(Poco::Net::HTTPServerRequest &req,
                       Poco::Net::HTTPServerResponse &res) final;

private:
    /// @brief Таблица с друзьями
    std::shared_ptr<FriendsTable> friendsTable_;
};
