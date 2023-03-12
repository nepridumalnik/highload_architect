#include <service/network/controllers/users/subcontrollers/user_register.hpp>

#include <service/database/models/users/users.hpp>
#include <service/database/models/users/users_auth.hpp>

#include <service/resources/jsons.hpp>

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

    if (http::verb::post != req.method())
    {
        res.result(http::status::bad_request);
    }
    else
    {
        const std::string body = buffers_to_string(req.body().data());

        UserRow user{};
        std::string error;

        if (!user.FromJson(body) || !usersTable_->Insert(user, error))
        {
            res.body() = nlohmann::json{{json_fields::Error, error}}.dump();
            res.result(http::status::bad_request);
        }
    }

    return true;
}
