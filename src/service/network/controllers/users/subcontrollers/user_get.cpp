#include <service/network/controllers/users/subcontrollers/user_get.hpp>

#include <service/database/models/users/users.hpp>

#include <service/resources/jsons.hpp>

#include <nlohmann/json.hpp>

using namespace boost::beast;

const std::string UserGetController::route_ = "/user/get";

UserGetController::UserGetController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

bool UserGetController::HandleRequest(const std::string &route,
                                      const http::request<http::dynamic_body> &req,
                                      websocket::response_type &res)
{
    if (route_.size() >= route.size() || route.find(route_) != 0)
    {
        return false;
    }

    for (auto it = route.back(); it != '/'; --it)
    {
        if (!std::isdigit(it))
        {
            return false;
        }
    }

    if (http::verb::get != req.method())
    {
        res.result(http::status::bad_request);
        return true;
    }

    const std::string substring = route.substr(route_.size() + 1);
    const size_t id = std::stoull(substring);

    getUserById(id, req, res);

    return true;
}

void UserGetController::getUserById(const size_t id,
                                    const http::request<http::dynamic_body> &req,
                                    websocket::response_type &res)
{
    UserRow user{};
    std::string error;

    if (!usersTable_->FindById(id, user, error))
    {
        res.body() = nlohmann::json{{json_fields::Error, error}}.dump();
        return res.result(http::status::not_found);
    }

    res.body() = user.ToString();
}
