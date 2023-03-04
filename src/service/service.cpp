#include <service/service.hpp>

#include <service/network/connection.hpp>

#include <soci/mysql/soci-mysql.h>
#include <soci/session.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <vector>

static const std::string ConnectionInfo = "host=localhost port=3306 dbname=main_database user=root password===PaSsWoRd==";

Service::Service()
{
    sql_ = std::make_shared<soci::session>(soci::mysql, ConnectionInfo);
    usersTable_ = std::make_shared<UsersTable>(sql_);
}

void Service::Run()
{
    boost::thread_group threadPull;

    // Stub
    // TODO: remove stub
    auto lambda = [](boost::asio::ip::tcp::socket socket) {};

    Connection connection{80, lambda};

    for (size_t i = 0; i < boost::thread::hardware_concurrency(); ++i)
    {
        threadPull.create_thread([&connection]()
                                 { connection(); });
    }

    threadPull.join_all();
}
