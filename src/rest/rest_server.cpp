#include <rest_server.hpp>

#include <http/request_handler.hpp>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Session.h>

namespace
{

    static const std::string ConnectionInfo = "host=localhost;port=3306;db=main_database;user=root;password===PaSsWoRd==";

} // namespace

RestServer::RestServer(std::shared_ptr<Poco::Data::SessionPool> pool)
{
    using namespace Poco::Data;

    static constexpr size_t maxPoolSize = 100;
    static constexpr size_t minPoolSize = 1;
    static constexpr size_t idleTime = 10;
    static constexpr size_t timeout = 10;

    MySQL::Connector::registerConnector();

    if (!pool)
    {
        pool = std::make_shared<SessionPool>(MySQL::Connector::KEY, ::ConnectionInfo, minPoolSize, maxPoolSize, idleTime, timeout);
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
