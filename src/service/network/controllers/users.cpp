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
