#include <http/controllers/user_login.hpp>

#include <models/users/users.hpp>
#include <models/users/users_auth.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>

#include <nlohmann/json.hpp>

UserLoginController::UserLoginController(std::shared_ptr<UsersTable> usersTable,
                                         std::shared_ptr<UsersAuthTable> authTable)
    : usersTable_{usersTable},
      authTable_{authTable}
{
}

void UserLoginController::handleRequest(Poco::Net::HTTPServerRequest &req,
                                        Poco::Net::HTTPServerResponse &res)
{
    const std::string method = req.getURI();

    if (method == methods::Get)
    {
        login(req, res);
    }
    else if (method == methods::Post)
    {
        authenticate(req, res);
    }
    else if (method == methods::Delete)
    {
        unauthorize(req, res);
    }
}

void UserLoginController::login(Poco::Net::HTTPServerRequest &req,
                                Poco::Net::HTTPServerResponse &res)
{
    std::istream &bodyStream = req.stream();
    std::string body;
    Poco::StreamCopier::copyToString(bodyStream, body);

    UserRowCond condition{};
    UserRow user{};
    std::string error;

    if (!condition.FromJson(body) || !usersTable_->FindByCondition(condition, user, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        return;
    }

    UserAuthRow auth{};
    auth.id = user.id;
    auth.token = user.Tokenize();

    if (!authTable_->Insert(auth, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        return;
    }

    nlohmann::json object{{json_fields::Token, auth.token}};
    res.send() << object.dump();
}

void UserLoginController::unauthorize(Poco::Net::HTTPServerRequest &req,
                                      Poco::Net::HTTPServerResponse &res)
{
    std::istream &bodyStream = req.stream();
    std::string body;
    Poco::StreamCopier::copyToString(bodyStream, body);
    const nlohmann::json object = nlohmann::json::parse(body);

    if (!object.contains(json_fields::Token) && !object[json_fields::Token].is_string())
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        return;
    }

    UserAuthRow auth{};
    std::string error;
    std::string token = object[json_fields::Token].get<std::string>();

    if (!authTable_->FindByCondition(token, auth, error) ||
        !authTable_->Delete(auth.id, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        return;
    }

    static const std::string statusSuccess = nlohmann::json{{json_fields::Status, json_fields::Success}}.dump();
    res.send() << statusSuccess;
}

void UserLoginController::authenticate(Poco::Net::HTTPServerRequest &req,
                                       Poco::Net::HTTPServerResponse &res)
{
    std::istream &bodyStream = req.stream();
    std::string body;
    Poco::StreamCopier::copyToString(bodyStream, body);
    const nlohmann::json object = nlohmann::json::parse(body);

    if (!object.contains(json_fields::Token) && !object[json_fields::Token].is_string())
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        return;
    }

    UserAuthRow auth{};
    std::string error;
    std::string token = object[json_fields::Token].get<std::string>();

    if (!authTable_->FindByCondition(token, auth, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        return;
    }

    static const std::string authorized = nlohmann::json{{json_fields::Status, json_fields::Authorized}}.dump();
    res.send() << authorized;
}
