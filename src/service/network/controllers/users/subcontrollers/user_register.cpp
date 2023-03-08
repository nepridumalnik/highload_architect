#include <service/network/controllers/users/subcontrollers/user_register.hpp>

#include <service/database/models/users/users.hpp>

#include <nlohmann/json.hpp>

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

    const std::string body = buffers_to_string(req.body().data());

    User user{};
    if (!user.fromJson(body))
    {
        res.result(http::status::bad_request);
        return true;
    }

    usersTable_->Insert(user);

    return true;
}
