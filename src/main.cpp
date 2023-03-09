#include <service/service.hpp>

#include <stdexcept>
#include <iostream>

int main(int argc, char const *argv[])
{
    try
    {
        Service service;
        service.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
