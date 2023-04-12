#include <http/controllers/make_friends.hpp>

#include <models/users/friends.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <nlohmann/json.hpp>

FriendsController::FriendsController(std::shared_ptr<FriendsTable> friendsTable)
    : friendsTable_{friendsTable}
{
}

void FriendsController::handleRequest(Poco::Net::HTTPServerRequest &req,
                                      Poco::Net::HTTPServerResponse &res)
{
}
