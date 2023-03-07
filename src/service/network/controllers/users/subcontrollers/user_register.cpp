#include <service/network/controllers/users/subcontrollers/user_register.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

UserRegisterController::UserRegisterController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

void UserRegisterController::HandleRequest(const http::request<http::dynamic_body> &req,
                                           websocket::response_type &res)
{
    res.body() = "<h1>Контроллер регистрации пользователей</h1>";
}
