#include <http/controllers/posts.hpp>

#include <models/users/posts.hpp>
#include <models/users/post_row.hpp>
#include <models/users/users.hpp>

#include <resources/methods.hpp>
#include <resources/jsons.hpp>

#include <utils/utils.hpp>

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/StreamCopier.h>

#include <nlohmann/json.hpp>

PostsController::PostsController(std::shared_ptr<PostsTable> postsTable,
                                 std::shared_ptr<UsersTable> usersTable)
    : postsTable_{std::move(postsTable)}, usersTable_{std::move(usersTable)}
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
    UserRow user;

    if (!usersTable_->FindById(row.user, user, error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << error;
        return;
    }

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
    std::string body;
    Poco::StreamCopier::copyToString(req.stream(), body);

    const nlohmann::json object = nlohmann::json::parse(body);

    if (object.contains(json_fields::Id) && object[json_fields::Id].is_number())
    {
        std::string error;
        PostRow row{};

        if (!postsTable_->FindById(object[json_fields::Id].get<int>(), row, error))
        {
            res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            res.send() << error;
            return;
        }

        res.send() << row.ToString();
    }
    else if (object.contains(json_fields::User) && object[json_fields::User].is_number() &&
             object.contains(json_fields::Count) && object[json_fields::Count].is_number_unsigned())
    {
        std::string error;
        std::vector<int> postIds;

        if (!postsTable_->FindPostsByUserId(
                postIds,
                object[json_fields::User].get<int>(),
                object[json_fields::Count].get<int>(),
                error))
        {
            res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            res.send() << error;
            return;
        }

        nlohmann::json array = nlohmann::json::array();

        for (const int &id : postIds)
        {
            array.push_back(id);
        }

        res.send() << array.dump();
    }
}

void PostsController::deletePost(Poco::Net::HTTPServerRequest &req,
                                 Poco::Net::HTTPServerResponse &res)
{
    std::string body;
    Poco::StreamCopier::copyToString(req.stream(), body);

    const nlohmann::json object = nlohmann::json::parse(body);

    if (!object.contains(json_fields::Id) || !object[json_fields::Id].is_number())
    {
        static const std::string error = nlohmann::json{{json_fields::Error, "Bad JSON format"}}.dump();
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << error;
        return;
    }

    std::string error;

    if (!postsTable_->Delete(object[json_fields::Id].get<int>(), error))
    {
        res.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        res.send() << error;
    }
    else
    {
        res.send();
    }
}
