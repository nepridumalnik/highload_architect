#include <http/controllers/user_search.hpp>

#include <models/users/users.hpp>

#include <resources/messages.hpp>
#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>

#include <nlohmann/json.hpp>

UserSearchController::UserSearchController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

void UserSearchController::handleRequest(Poco::Net::HTTPServerRequest &req,
                                         Poco::Net::HTTPServerResponse &res)
{
    if (methods::Post != req.getMethod())
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        return;
    }

    searchUsers(req, res);
}

void UserSearchController::searchUsers(Poco::Net::HTTPServerRequest &req,
                                       Poco::Net::HTTPServerResponse &res)
{
    std::istream &bodyStream = req.stream();
    std::string body;
    Poco::StreamCopier::copyToString(bodyStream, body);
    const nlohmann::json object = nlohmann::json::parse(body);

    if ((!object.contains(json_fields::FirstName) || !object[json_fields::FirstName].is_string()) ||
        (!object.contains(json_fields::SecondName) || !object[json_fields::SecondName].is_string()))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, messages::WrongJsonFormat}}.dump();
        return;
    }

    std::string firstName = object[json_fields::FirstName].get<std::string>();
    std::string secondName = object[json_fields::SecondName].get<std::string>();
    std::string error;
    std::vector<UserRow> users;

    if (!usersTable_->SearchByNames(users, firstName, secondName, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        return;
    }

    if (users.empty())
    {
        static const std::string empty = "{}";
        res.send() << empty;
        return;
    }

    nlohmann::json array = nlohmann::json::array();

    for (const UserRow &user : users)
    {
        array.push_back(user.ToJson());
    }

    res.send() << array.dump();
}
