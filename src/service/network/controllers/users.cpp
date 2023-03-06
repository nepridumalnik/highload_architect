#include <service/network/controllers/users.hpp>

using namespace boost::beast;

UsersController::UsersController(std::shared_ptr<soci::session> sql)
{
    usersTable_ = std::make_unique<UsersTable>(sql);
}

void UsersController::HandleRequest(const http::request<http::dynamic_body> &req,
                                    websocket::response_type &res)
{
    res.body() = "<h1>Hello, User!</h1>";
}

bool UsersController::IsRouteHandled(const std::string &route)
{
    static const std::string loginRoute = "/login";
    static const std::string userRegisterRoute = "/user/register";
    static const std::string userGetIdRoute = "/user/get/";

    if (route == loginRoute)
    {
        return true;
    }

    if (route == userRegisterRoute)
    {
        return true;
    }

    if (route.size() > userGetIdRoute.size() && route.find(userGetIdRoute) == 0)
    {
        for (auto it = route.back(); it != '/'; --it)
        {
            if (!std::isdigit(it))
            {
                return false;
            }
        }

        return true;
    }

    return false;
}
