#include <service/service.hpp>

#include <service/network/controllers/users/users.hpp>
#include <service/network/connection.hpp>
#include <service/network/router.hpp>

#include <service/resources/connection_parameters.hpp>
#include <service/settings/settings_reader.hpp>

#include <soci/mysql/soci-mysql.h>
#include <soci/session.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <vector>

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
            connectionInfo += ' ';
        }

        connectionInfo += parameter.first + '=' + newParameter;
    }

    return connectionInfo;
}

Service::Service()
{
    const std::string connectionInfo = GetConnectionInfo();

    sql_ = std::make_shared<soci::session>(soci::mysql, connectionInfo);
    router_.AddController(std::make_unique<UsersController>(sql_));
}

void Service::Run()
{
    boost::thread_group threadPull;

    const auto callback = [&router = this->router_](boost::asio::ip::tcp::socket &socket) -> void
    {
        router(socket);
    };

    Connection connection{80, callback};

    for (size_t i = 0; i < boost::thread::hardware_concurrency(); ++i)
    {
        threadPull.create_thread([&connection]()
                                 { connection(); });
    }

    threadPull.join_all();
}
