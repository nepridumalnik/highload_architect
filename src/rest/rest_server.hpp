#pragma once

#include <models/users/users.hpp>

#include <helpers/macro.hpp>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/ServerApplication.h>

#include <memory>

class RestServer : public Poco::Util::ServerApplication
{
public:
    EXPORT RestServer(std::shared_ptr<Poco::Data::SessionPool> pool = nullptr);
    void EXPORT Start();
    void EXPORT WaitForTerminationRequest();

private:
    Poco::SharedPtr<Poco::Net::HTTPServer> server_;
    Poco::Net::HTTPRequestHandlerFactory::Ptr requestHandler_;
    std::shared_ptr<Poco::Data::SessionPool> pool_;
};
