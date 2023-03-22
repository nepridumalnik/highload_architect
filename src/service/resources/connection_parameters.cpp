#include <service/resources/connection_parameters.hpp>

/// @brief Настройки по умолчанию
namespace connection_parameters
{
    const std::string DefaultConnectionInfo = "host=mysql port=3306 dbname=main_database user=root password===PaSsWoRd==";
    const std::string Host = "host";
    const std::string DefaultHost = "mysql";
    const std::string Port = "port";
    const std::string DefaultPort = "3306";
    const std::string Database = "db";
    const std::string DefaultDatabase = "main_database";
    const std::string User = "user";
    const std::string DefaultUser = "root";
    const std::string Password = "password";
    const std::string DefaultPassword = "==PaSsWoRd==";
} // namespace connection_parameters