#include <http/controllers/user_get.hpp>

#include <models/users/users.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <nlohmann/json.hpp>

UserGetController::UserGetController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

void UserGetController::handleRequest(Poco::Net::HTTPServerRequest &req,
                                      Poco::Net::HTTPServerResponse &res)
{
    const std::string route = req.getURI();
    size_t counter = 0;

    for (counter = route.size() - 1; counter > 0; --counter)
    {
        if (route[counter] == '/')
        {
            break;
        }
        if (!std::isdigit(route[counter]))
        {
            return;
        }
    }

    if (counter == route.size() || methods::Get != req.getMethod())
    {
        return;
    }

    const std::string substring = route.substr(counter + 1);
    const size_t id = std::stoull(substring);

    getUserById(id, req, res);
}

void UserGetController::getUserById(const size_t id,
                                    Poco::Net::HTTPServerRequest &req,
                                    Poco::Net::HTTPServerResponse &res)
{
    UserRow user{};
    std::string error;

    if (!usersTable_->FindById(id, user, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
    }

    res.send() << user.ToString();
}
