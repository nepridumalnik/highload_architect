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

bool UsersController::HandleRequest(const std::string &route,
                                    const http::request<http::dynamic_body> &req,
                                    websocket::response_type &res)
{
    for (auto &subcontroller : subcontrollers_)
    {
        if (subcontroller->HandleRequest(route, req, res))
        {
            return true;
        }
    }

    return false;
}
