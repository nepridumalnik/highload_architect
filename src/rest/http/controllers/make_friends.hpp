#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <memory>

class FriendsTable;
class FriendRow;

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
    /// @brief Добавить друга
    /// @param row Друг
    /// @param res Ответ
    void postFriend(FriendRow &row, Poco::Net::HTTPServerResponse &res);

    /// @brief Получить друга
    /// @param req Запрос
    /// @param res Ответ
    void getFriend(Poco::Net::HTTPServerRequest &req,
                   Poco::Net::HTTPServerResponse &res);

    /// @brief Удалить друга
    /// @param row Друг
    /// @param res Ответ
    void deleteFriend(FriendRow &row, Poco::Net::HTTPServerResponse &res);

private:
    /// @brief Таблица с друзьями
    std::shared_ptr<FriendsTable> friendsTable_;
};
