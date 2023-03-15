#include <service/service.hpp>

#include <service/network/controllers/users/users.hpp>
#include <service/network/connection.hpp>
#include <service/network/router.hpp>

#include <soci/mysql/soci-mysql.h>
#include <soci/session.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

static const std::string ConnectionInfo = "host=mysql port=3306 dbname=main_database user=root password===PaSsWoRd==";

Service::Service()
{
    sql_ = std::make_shared<soci::session>(soci::mysql, ConnectionInfo);
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
