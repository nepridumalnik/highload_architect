#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <memory>

class UsersTable;
class PostsTable;
class PostRow;

/// @brief Контроллер работы с постами
class PostsController : public Poco::Net::HTTPRequestHandler
{
public:
    /// @brief Конструктор
    explicit PostsController(std::shared_ptr<PostsTable> postsTable,
                             std::shared_ptr<UsersTable> usersTable);

    /// @see HTTPRequestHandler
    void handleRequest(Poco::Net::HTTPServerRequest &req,
                       Poco::Net::HTTPServerResponse &res) final;

private:
    /// @brief Добавить друга
    /// @param req Запрос
    /// @param res Ответ
    void postPost(Poco::Net::HTTPServerRequest &req,
                  Poco::Net::HTTPServerResponse &res);

    /// @brief Получить друга
    /// @param req Запрос
    /// @param res Ответ
    void getPost(Poco::Net::HTTPServerRequest &req,
                 Poco::Net::HTTPServerResponse &res);

    /// @brief Удалить друга
    /// @param req Запрос
    /// @param res Ответ
    void deletePost(Poco::Net::HTTPServerRequest &req,
                    Poco::Net::HTTPServerResponse &res);

private:
    /// @brief Таблица с друзьями
    std::shared_ptr<PostsTable> postsTable_;

    /// @brief Таблица с пользователями
    std::shared_ptr<UsersTable> usersTable_;
};
