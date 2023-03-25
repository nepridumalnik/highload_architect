#include <rest_server.hpp>

#include <http/request_handler.hpp>

#include <resources/connection_parameters.hpp>
#include <settings/settings_reader.hpp>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Session.h>

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

RestServer::RestServer(std::shared_ptr<Poco::Data::SessionPool> pool)
{
    using namespace Poco::Data;

    static constexpr size_t maxPoolSize = 100;
    static constexpr size_t minPoolSize = 1;
    static constexpr size_t idleTime = 10;
    static constexpr size_t timeout = 10;

    const std::string connectionInfo = GetConnectionInfo();

    MySQL::Connector::registerConnector();

    if (!pool)
    {
        pool = std::make_shared<SessionPool>(MySQL::Connector::KEY, connectionInfo, minPoolSize, maxPoolSize, idleTime, timeout);
    }

    requestHandler_ = Poco::makeShared<RequestHandler>(pool);
    server_ = Poco::makeShared<Poco::Net::HTTPServer>(requestHandler_);
}

void RestServer::Start()
{
    server_->start();
}

void RestServer::WaitForTerminationRequest()
{
    waitForTerminationRequest();
}
