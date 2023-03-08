#include <service/network/controllers/users/subcontrollers/user_get.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

const std::string UserGetController::route_ = "/user/get";

UserGetController::UserGetController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

bool UserGetController::HandleRequest(const std::string &route,
                                      const http::request<http::dynamic_body> &req,
                                      websocket::response_type &res)
{
    if (route_.size() >= route.size() || route.find(route_) != 0)
    {
        return false;
    }

    for (auto it = route.back(); it != '/'; --it)
    {
        if (!std::isdigit(it))
        {
            return false;
        }
    }

    res.body() = "<h1>Контроллер получения пользователей</h1>";

    return true;
}
