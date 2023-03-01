#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

#include <iostream>

static const std::string ConnectionInfo = "host=localhost port=3306 dbname=main_database user=root password===PaSsWoRd==";

static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS Users (\n"
                                       "ID INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,\n"
                                       "User VARCHAR(50) NOT NULL,\n"
                                       "Password VARCHAR(50) NOT NULL\n"
                                       ") ENGINE=InnoDB";

int main(int argc, char const *argv[])
{
    try
    {
        soci::session sql{soci::mysql, ConnectionInfo};

        sql << CreateTable;
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_ << ", " << e.what() << std::endl;
    }
    catch (soci::soci_error const &e)
    {
        std::cerr << "SOCI error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
