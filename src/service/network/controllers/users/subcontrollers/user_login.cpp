#include <service/network/controllers/users/subcontrollers/user_login.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

const std::string UserLoginController::route_ = "/login";

UserLoginController::UserLoginController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

void UserLoginController::HandleRequest(const http::request<http::dynamic_body> &req,
                                           websocket::response_type &res)
{
    res.body() = "<h1>Контроллер авторизации пользователей</h1>";
}

bool UserLoginController::HasRoute(const std::string &route)
{
    return route_ == route;
}
