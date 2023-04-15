#include <http/controllers/make_friends.hpp>

#include <models/users/friends.hpp>
#include <models/users/friend_row.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

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
    const std::string method = req.getMethod();

    if (method != methods::Post && method != methods::Delete)
    {
        res.send();
        return;
    }

    std::istream &bodyStream = req.stream();
    std::string body;
    Poco::StreamCopier::copyToString(bodyStream, body);

    FriendRow row{};
    std::string error;

    if (method == methods::Post)
    {
        if (!row.FromJson(body) || !friendsTable_->Insert(row, error))
        {
            res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
            return;
        }
    }
    else
    {
        FriendRow f{};
        if (!row.FromJson(body) ||
            !friendsTable_->FindByCondition(row, f, error) ||
            !friendsTable_->Delete(f.id, error))
        {
            res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            res.send() << nlohmann::json{{json_fields::Error, error}}.dump();
            return;
        }
    }

    res.send();
}
