#include <http/request_handler.hpp>

#include <http/controllers/user_get.hpp>
#include <http/controllers/user_login.hpp>
#include <http/controllers/user_register.hpp>
#include <http/controllers/user_search.hpp>
#include <http/controllers/make_friends.hpp>
#include <http/controllers/posts.hpp>

#include <models/users/users.hpp>
#include <models/users/users_auth.hpp>
#include <models/users/friends.hpp>
#include <models/users/posts.hpp>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Data/SessionPool.h>

RequestHandler::RequestHandler(std::shared_ptr<Poco::Data::SessionPool> pool)
    : pool_{std::move(pool)}
{
    usersTable_ = std::make_shared<UsersTable>(pool_);
    authTable_ = std::make_shared<UsersAuthTable>(pool_);
    friendsTable_ = std::make_shared<FriendsTable>(pool_);
    postsTable_ = std::make_shared<PostsTable>(pool_);

    routing_.push_back({"/user/get", [&usersTable = usersTable_, this]()
                        { return new UserGetController{usersTable}; }});

    routing_.push_back({"/login", [&usersTable = usersTable_, &authTable = authTable_, this]()
                        { return new UserLoginController{usersTable, authTable}; }});

    routing_.push_back({"/user/register", [&usersTable = usersTable_, this]()
                        { return new UserRegisterController{usersTable}; }});

    routing_.push_back({"/user/search", [&usersTable = usersTable_, this]()
                        { return new UserSearchController{usersTable}; }});

    routing_.push_back({"/posts", [&postsTable = postsTable_, this]()
                        { return new PostsController{postsTable}; }});

    routing_.push_back({
        "/friends",
        [&friendsTable = friendsTable_]()
        { return new FriendsController{friendsTable}; },
    });
}

Poco::Net::HTTPRequestHandler *RequestHandler::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
{
    const std::string requestedUri = request.getURI();

    for (const auto &it : routing_)
    {
        if (0 == requestedUri.find(it.first))
        {
            return it.second();
        }
    }

    return nullptr;
}
