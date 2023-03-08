#include <service/network/controllers/users/subcontrollers/user_get.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

const std::string UserGetController::route_ = "/user/get";

UserGetController::UserGetController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

void UserGetController::HandleRequest(const http::request<http::dynamic_body> &req,
                                      websocket::response_type &res)
{
    res.body() = "<h1>Контроллер получения пользователей</h1>";
}

bool UserGetController::HasRoute(const std::string &route)
{
    if (route_.size() < route.size() && route.find(route_) == 0)
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
