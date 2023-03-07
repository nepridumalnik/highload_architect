#include <service/network/controllers/users/users.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

const std::string UsersController::loginRoute = "/login";

const std::string UsersController::userRegisterRoute = "/user/register";

const std::string UsersController::userGetIdRoute = "/user/get/";

UsersController::UsersController(std::shared_ptr<soci::session> sql)
{
    usersTable_ = std::make_unique<UsersTable>(sql);
    userRegister_ = std::make_unique<UserRegisterController>(usersTable_);
}

void UsersController::HandleRequest(const http::request<http::dynamic_body> &req,
                                    websocket::response_type &res)
{
    const std::string route = req.target();

    if (route == userRegisterRoute)
    {
        userRegister_->HandleRequest(req, res);
        return;
    }

    res.body() = "<h1>Unhandled route</h1>";
}

bool UsersController::HasRoute(const std::string &route)
{
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
