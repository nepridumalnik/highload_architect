#include <service/network/controllers/users/subcontrollers/user_search.hpp>

#include <service/database/models/users/users.hpp>

#include <service/resources/jsons.hpp>
#include <service/resources/messages.hpp>

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

    if (http::verb::post != req.method())
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
    const std::string body = buffers_to_string(req.body().data());
    const nlohmann::json object = nlohmann::json::parse(body);

    if ((!object.contains(json_fields::FirstName) && !object[json_fields::FirstName].is_string()) ||
        (!object.contains(json_fields::SecondName) && !object[json_fields::SecondName].is_string()))
    {
        res.body() = nlohmann::json{{json_fields::Error, messages::WrongJsonFormat}}.dump();
        return res.result(http::status::bad_request);
    }

    const std::string firstName = object[json_fields::FirstName].get<std::string>();
    const std::string secondName = object[json_fields::SecondName].get<std::string>();
    std::string error;
    std::vector<UserRow> users;

    if (!usersTable_->SearchByNames(users, firstName, secondName, error))
    {
        res.body() = nlohmann::json{{json_fields::Error, error}}.dump();
        return res.result(http::status::bad_request);
    }

    if (users.empty())
    {
        static const std::string empty = "{}";
        res.body() = empty;
        return;
    }

    nlohmann::json usersJson{};

    for (const UserRow &user : users)
    {
        usersJson.push_back(user.ToJson());
    }

    res.body() = usersJson.dump();
}
