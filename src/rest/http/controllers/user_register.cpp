#include <http/controllers/user_register.hpp>

#include <models/users/users.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>

#include <nlohmann/json.hpp>

UserRegisterController::UserRegisterController(std::shared_ptr<UsersTable> usersTable)
    : usersTable_{usersTable}
{
}

void UserRegisterController::handleRequest(Poco::Net::HTTPServerRequest &req,
                                           Poco::Net::HTTPServerResponse &res)
{
    if (methods::Post != req.getMethod())
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        return;
    }

    std::istream &bodyStream = req.stream();
    std::string body;
    Poco::StreamCopier::copyToString(bodyStream, body);
    const nlohmann::json object = nlohmann::json::parse(body);

    UserRow user{};
    std::string error;

    if (!user.FromJson(body) || !usersTable_->Insert(user, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
    }
}
