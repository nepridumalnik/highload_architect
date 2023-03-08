#include <service/network/controllers/users/subcontrollers/user_register.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

const std::string UserRegisterController::route_ = "/user/register";

UserRegisterController::UserRegisterController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

bool UserRegisterController::HandleRequest(const std::string &route,
                                           const http::request<http::dynamic_body> &req,
                                           websocket::response_type &res)
{
    if (route_ != route)
    {
        return false;
    }

    res.body() = "<h1>Контроллер регистрации пользователей</h1>";

    return true;
}
