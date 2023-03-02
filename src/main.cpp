#include <service/service.hpp>

#include <soci/mysql/soci-mysql.h>
#include <soci/soci.h>

#include <iostream>

int main(int argc, char const *argv[])
{
    try
    {
        Service service;
        service.Run();
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
