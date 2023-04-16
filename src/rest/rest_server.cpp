#include <rest_server.hpp>

#include <http/request_handler.hpp>

#include <resources/connection_parameters.hpp>
#include <settings/settings_reader.hpp>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Session.h>

#include <sw/redis++/redis++.h>

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
    static constexpr size_t idleTime = 1000;
    static constexpr size_t timeout = 100;

    const std::string connectionInfo = GetConnectionInfo();

    std::shared_ptr<sw::redis::Redis> redis = std::make_shared<sw::redis::Redis>("redis://localhost:6379");

    MySQL::Connector::registerConnector();

    if (!pool)
    {
        pool = std::make_shared<SessionPool>(MySQL::Connector::KEY, connectionInfo, minPoolSize, maxPoolSize, idleTime, timeout);
    }

    requestHandler_ = Poco::makeShared<RequestHandler>(std::move(pool), std::move(redis));

    Poco::Net::HTTPServerParams::Ptr params = new Poco::Net::HTTPServerParams{};
    params->setMaxThreads(1000);
    params->setMaxQueued(1000);
    params->setThreadIdleTime(10000);
    params->setThreadPriority(Poco::Thread::PRIO_HIGHEST);

    server_ = Poco::makeShared<Poco::Net::HTTPServer>(requestHandler_, 80, params);
}

void RestServer::Start()
{
    server_->start();
}

void RestServer::WaitForTerminationRequest()
{
    waitForTerminationRequest();
}
