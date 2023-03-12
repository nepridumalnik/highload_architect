#include <service/network/controllers/users/subcontrollers/user_login.hpp>

#include <service/database/models/users/users.hpp>

using namespace boost::beast;

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
}

void UserLoginController::unauthorize(const http::request<http::dynamic_body> &req,
                                      boost::beast::websocket::response_type &res)
{
}

void UserLoginController::authenticate(const http::request<http::dynamic_body> &req,
                                       websocket::response_type &res)
{
}
