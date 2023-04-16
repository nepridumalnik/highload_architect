#include <http/controllers/make_friends.hpp>

#include <models/users/friends.hpp>
#include <models/users/friend_row.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <utils/utils.hpp>

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/StreamCopier.h>

#include <nlohmann/json.hpp>

FriendsController::FriendsController(std::shared_ptr<FriendsTable> friendsTable)
    : friendsTable_{friendsTable}
{
}

void FriendsController::handleRequest(Poco::Net::HTTPServerRequest &req,
                                      Poco::Net::HTTPServerResponse &res)
{
    const HttpMethod method = GetMethod(req.getMethod());

    if (method != HttpMethod::Post &&
        method != HttpMethod::Delete)
    {
        if (method == HttpMethod::Get)
        {
            getFriend(req, res);
            return;
        }
        else
        {
            res.send();
            return;
        }
    }

    std::string body;
    Poco::StreamCopier::copyToString(req.stream(), body);

    FriendRow row{};

    if (!row.FromJson(body))
    {
        static const std::string error = nlohmann::json{{json_fields::Error, "Bad JSON format"}}.dump();
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << error;
    }

    if (HttpMethod::Post == method)
    {
        postFriend(row, res);
    }
    else
    {
        deleteFriend(row, res);
    }

    res.send();
}

void FriendsController::postFriend(FriendRow &row, Poco::Net::HTTPServerResponse &res)
{
    std::string error;
    if (!friendsTable_->Insert(row, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
    }
}

void FriendsController::deleteFriend(FriendRow &row, Poco::Net::HTTPServerResponse &res)
{
    std::string error;
    FriendRow f{};
    if (!friendsTable_->FindByCondition(row, f, error) ||
        !friendsTable_->Delete(f.id, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        return;
    }
}

void FriendsController::getFriend(Poco::Net::HTTPServerRequest &req,
                                  Poco::Net::HTTPServerResponse &res)
{
    std::string body;
    Poco::StreamCopier::copyToString(req.stream(), body);

    const nlohmann::json object = nlohmann::json::parse(body);

    if (!object.contains(json_fields::User) || !object[json_fields::User].is_number())
    {
        static const std::string error = nlohmann::json{{json_fields::Error, "Bad JSON format"}}.dump();
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << error;
        return;
    }

    if (!object.contains(json_fields::Friend) || !object[json_fields::Friend].is_number())
    {
        std::string error;
        std::vector<FriendRow> friends;

        if (!friendsTable_->GetAllFriends(friends, object[json_fields::User].get<int>(), error))
        {
            res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        }
        else
        {
            nlohmann::json array = nlohmann::json::array();

            for (const FriendRow &f : friends)
            {
                array.push_back(f.ToJson());
            }

            res.send() << array.dump();
        }

        return;
    }

    FriendRow row{};
    FriendRow condition{};
    std::string error;

    condition.user = object[json_fields::User].get<int>();
    condition.other = object[json_fields::Friend].get<int>();

    if (!friendsTable_->FindByCondition(condition, row, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
        return;
    }

    res.send() << row.ToString();
}
