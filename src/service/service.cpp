#include <service/service.hpp>

#include <service/network/controllers/users/users.hpp>
#include <service/network/connection.hpp>
#include <service/network/router.hpp>

#include <service/resources/connection_parameters.hpp>
#include <service/settings/settings_reader.hpp>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Session.h>

#include <vector>
#include <iostream>

std::string GetConnectionInfo()
{
    static const std::vector<std::pair<const std::string, const std::string>> Parameters{
        {connection_parameters::Host, connection_parameters::DefaultHost},
        {connection_parameters::Port, connection_parameters::DefaultPort},
        {connection_parameters::Database, connection_parameters::DefaultDatabase},
        {connection_parameters::User, connection_parameters::DefaultUser},
        {connection_parameters::Password, connection_parameters::DefaultPassword},
    };

    SettingsReader reader;

    if (!reader.ReadFile("settings.json"))
    {
        return connection_parameters::DefaultConnectionInfo;
    }

    std::string connectionInfo;

    for (const auto &parameter : Parameters)
    {
        std::string newParameter = reader.GetParameter<std::string>(parameter.first);

        if (newParameter.empty())
        {
            newParameter = parameter.second;
        }

        if (!connectionInfo.empty())
        {
            connectionInfo += ';';
        }

        connectionInfo += parameter.first + '=' + newParameter;
    }

    return connectionInfo;
}

Service::Service()
{
    using namespace Poco::Data;

    static constexpr size_t maxPoolSize = 100;
    static constexpr size_t minPoolSize = 1;
    static constexpr size_t idleTime = 10;
    static constexpr size_t timeout = 10;

    const std::string connectionInfo = GetConnectionInfo();
    MySQL::Connector::registerConnector();

    std::shared_ptr<SessionPool> pool =
        std::make_shared<SessionPool>(MySQL::Connector::KEY, connectionInfo, minPoolSize, maxPoolSize, idleTime, timeout);

    router_.AddController(std::make_unique<UsersController>(pool));
}

void Service::Run()
{
    const auto callback = [&router = this->router_](boost::asio::ip::tcp::socket &socket) -> void
    {
        router(socket);
    };

    Connection connection{80, callback};
    connection.Run();
}
