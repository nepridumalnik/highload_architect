#include <service/network/controllers/users/users.hpp>

#include <service/network/controllers/users/subcontrollers/user_get.hpp>
#include <service/network/controllers/users/subcontrollers/user_login.hpp>
#include <service/network/controllers/users/subcontrollers/user_register.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

UsersController::UsersController(std::shared_ptr<soci::session> sql)
{
    usersTable_ = std::make_unique<UsersTable>(sql);
    subcontrollers_.push_back(std::make_unique<UserGetController>(usersTable_));
    subcontrollers_.push_back(std::make_unique<UserLoginController>(usersTable_));
    subcontrollers_.push_back(std::make_unique<UserRegisterController>(usersTable_));
}

void UsersController::HandleRequest(const http::request<http::dynamic_body> &req,
                                    websocket::response_type &res)
{
    const std::string route = req.target();

    for (auto &subcontroller : subcontrollers_)
    {
        if (subcontroller->HasRoute(route))
        {
            subcontroller->HandleRequest(req, res);
            return;
        }
    }

    res.body() = "<h1>Unhandled route</h1>";
}

bool UsersController::HasRoute(const std::string &route)
{
    for (auto &subcontroller : subcontrollers_)
    {
        if (subcontroller->HasRoute(route))
        {
            return true;
        }
    }

    return false;
}
