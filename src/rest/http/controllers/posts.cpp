#include <http/controllers/posts.hpp>

#include <models/users/friends.hpp>
#include <models/users/friend_row.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <utils/utils.hpp>

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/StreamCopier.h>

#include <nlohmann/json.hpp>

PostsController::PostsController(std::shared_ptr<PostsTable> postsTable)
    : postsTable_{postsTable}
{
}

void PostsController::handleRequest(Poco::Net::HTTPServerRequest &req,
                                    Poco::Net::HTTPServerResponse &res)
{
    const HttpMethod method = GetMethod(req.getMethod());

    switch (method)
    {
    case HttpMethod::Post:
        postPost(req, res);
        return;

    case HttpMethod::Get:
        getPost(req, res);
        return;

    case HttpMethod::Delete:
        deletePost(req, res);
        return;

    default:
        static const std::string error = nlohmann::json{{json_fields::Error, "Wrong method"}}.dump();
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        res.send() << error;
        return;
    }
}

void PostsController::postPost(Poco::Net::HTTPServerRequest &req,
                               Poco::Net::HTTPServerResponse &res)
{
}

void PostsController::getPost(Poco::Net::HTTPServerRequest &req,
                              Poco::Net::HTTPServerResponse &res)
{
}

void PostsController::deletePost(Poco::Net::HTTPServerRequest &req,
                                 Poco::Net::HTTPServerResponse &res)
{
}
