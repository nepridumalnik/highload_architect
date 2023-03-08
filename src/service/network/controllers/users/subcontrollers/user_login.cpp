#include <service/network/controllers/users/subcontrollers/user_login.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

const std::string UserLoginController::route_ = "/login";

UserLoginController::UserLoginController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

bool UserLoginController::HandleRequest(const std::string &route,
                                        const http::request<http::dynamic_body> &req,
                                        websocket::response_type &res)
{
    if (route_ != route)
    {
        return false;
    }

    res.body() = "<h1>Контроллер авторизации пользователей</h1>";

    return true;
}
