#include <service/service.hpp>

#include <soci/mysql/soci-mysql.h>
#include <soci/session.h>

static const std::string ConnectionInfo = "host=localhost port=3306 dbname=main_database user=root password===PaSsWoRd==";

Service::Service()
{
    sql_ = std::make_shared<soci::session>(soci::mysql, ConnectionInfo);
    usersTable_ = std::make_shared<UsersTable>(sql_);
}

void Service::Run()
{
}
