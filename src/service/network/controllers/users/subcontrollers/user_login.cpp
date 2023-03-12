#include <service/network/controllers/users/subcontrollers/user_login.hpp>

#include <service/database/models/users/users.hpp>
#include <service/database/models/users/users_auth.hpp>
#include <service/database/models/users/user_find_condition.hpp>

#include <nlohmann/json.hpp>

using namespace boost::beast;

namespace json_fields
{
    static const std::string Token = "token";
    static const std::string Status = "status";
    static const std::string Authorized = "authorized";
} // json_fields

const std::string UserLoginController::route_ = "/login";

UserLoginController::UserLoginController(std::shared_ptr<UsersTable> usersTable,
                                         std::shared_ptr<UsersAuthTable> authTable)
    : usersTable_{usersTable},
      authTable_{authTable}
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

    switch (req.method())
    {
    case http::verb::post:
        login(req, res);
        break;

    case http::verb::get:
        authenticate(req, res);
        break;

    case http::verb::delete_:
        unauthorize(req, res);
        break;

    default:
        res.result(http::status::bad_request);
        break;
    }

    return true;
}

void UserLoginController::login(const http::request<http::dynamic_body> &req,
                                websocket::response_type &res)
{
    const std::string body = buffers_to_string(req.body().data());

    UserRowCond condition{};
    UserRow user{};

    if (!condition.FromJson(body) || !usersTable_->FindByCondition(condition, user))
    {
        return res.result(http::status::bad_request);
    }

    UserAuthRow auth{};
    auth.id = user.id;
    auth.token = user.Tokenize();

    if (!authTable_->Insert(auth))
    {
        return res.result(http::status::bad_request);
    }
    else
    {
        nlohmann::json object{{json_fields::Token, auth.token}};
        res.body() = object.dump();
    }
}

void UserLoginController::unauthorize(const http::request<http::dynamic_body> &req,
                                      boost::beast::websocket::response_type &res)
{
    const std::string body = buffers_to_string(req.body().data());
    const nlohmann::json object = nlohmann::json::parse(body);

    if (!object.contains(json_fields::Token) && !object[json_fields::Token].is_string())
    {
        return res.result(http::status::bad_request);
    }

    UserAuthRow auth;

    if (!authTable_->FindByCondition(object[json_fields::Token], auth) ||
        !authTable_->Delete(auth.id))
    {
        return res.result(http::status::bad_request);
    }
}

void UserLoginController::authenticate(const http::request<http::dynamic_body> &req,
                                       websocket::response_type &res)
{
    const std::string body = buffers_to_string(req.body().data());
    const nlohmann::json object = nlohmann::json::parse(body);

    if (!object.contains(json_fields::Token) && !object[json_fields::Token].is_string())
    {
        return res.result(http::status::bad_request);
    }

    UserAuthRow auth;

    if (!authTable_->FindByCondition(object[json_fields::Token], auth))
    {
        return res.result(http::status::bad_request);
    }

    static const std::string authorized = nlohmann::json{{json_fields::Status, json_fields::Authorized}}.dump();
    res.body() = authorized;
}
