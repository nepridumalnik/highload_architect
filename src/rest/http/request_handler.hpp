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

class UsersTable;
class UsersAuthTable;

class RequestHandler : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    explicit RequestHandler(std::shared_ptr<Poco::Data::SessionPool> pool);

    Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;

private:
    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Таблица с данными авторизаций
    std::shared_ptr<UsersAuthTable> authTable_;

    /// @brief Пул соединений
    std::shared_ptr<Poco::Data::SessionPool> pool_;

    /// @brief Маршруты
    std::vector<std::pair<std::string, std::function<Poco::Net::HTTPRequestHandler *()>>> routing_;
};
