#include <rest_server.hpp>

int main(int argc, char const *argv[])
{
    RestServer server;
    server.Start();

    server.WaitForTerminationRequest();

    return 0;
}
