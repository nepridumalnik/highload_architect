#include <service/network/controllers/users/subcontrollers/user_search.hpp>

#include <service/database/models/users/users.hpp>

#include <service/resources/jsons.hpp>

#include <nlohmann/json.hpp>

using namespace boost::beast;

const std::string UserSearchController::route_ = "/user/search";

UserSearchController::UserSearchController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

bool UserSearchController::HandleRequest(const std::string &route, const boost::beast::http::request<boost::beast::http::dynamic_body> &req, boost::beast::websocket::response_type &res)
{
    if (route_ != route)
    {
        return false;
    }

    if (http::verb::get != req.method())
    {
        res.result(http::status::bad_request);
        return true;
    }

    searchUsers(req, res);

    return true;
}

void UserSearchController::searchUsers(const boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                                       boost::beast::websocket::response_type &res)
{
}
