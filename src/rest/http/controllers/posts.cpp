#include <http/controllers/posts.hpp>

#include <models/users/posts.hpp>
#include <models/users/post_row.hpp>

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
    std::string body;
    Poco::StreamCopier::copyToString(req.stream(), body);

    const nlohmann::json object = nlohmann::json::parse(body);

    if (!object.contains(json_fields::User) || !object[json_fields::User].is_number() ||
        !object.contains(json_fields::Post) || !object[json_fields::Post].is_string())
    {
        static const std::string error = nlohmann::json{{json_fields::Error, "Bad JSON format"}}.dump();
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << error;
        return;
    }

    std::string error;
    PostRow row{};
    row.user = object[json_fields::User].get<int>();
    row.post = object[json_fields::Post].get<std::string>();

    if (!postsTable_->Insert(row, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << error;
    }
    else
    {
        res.send();
    }
}

void PostsController::getPost(Poco::Net::HTTPServerRequest &req,
                              Poco::Net::HTTPServerResponse &res)
{
}

void PostsController::deletePost(Poco::Net::HTTPServerRequest &req,
                                 Poco::Net::HTTPServerResponse &res)
{
}
