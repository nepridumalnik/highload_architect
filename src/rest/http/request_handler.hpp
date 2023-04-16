#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <functional>
#include <memory>
#include <vector>

namespace Poco::Data
{
    class SessionPool;
} // namespace Poco::Data

namespace sw::redis
{
    class Redis;
} // namespace sw::redis

class UsersTable;
class UsersAuthTable;
class FriendsTable;
class PostsTable;

class RequestHandler : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    explicit RequestHandler(std::shared_ptr<Poco::Data::SessionPool> pool,
                            std::shared_ptr<sw::redis::Redis> redis);

    Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Таблица с данными авторизаций
    std::shared_ptr<UsersAuthTable> authTable_;

    /// @brief Таблица с друзьями
    std::shared_ptr<FriendsTable> friendsTable_;

    /// @brief Таблица с постами
    std::shared_ptr<PostsTable> postsTable_;

    /// @brief Пул соединений
    std::shared_ptr<Poco::Data::SessionPool> pool_;

    /// @brief Клиент редиски
    std::shared_ptr<sw::redis::Redis> redis_;

    /// @brief Маршруты
    std::vector<std::pair<std::string, std::function<Poco::Net::HTTPRequestHandler *()>>> routing_;
};
