#include <models/users/posts.hpp>

#include <utils/utils.hpp>

#include <resources/messages.hpp>
#include <resources/jsons.hpp>

#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Transaction.h>
#include <Poco/Data/Connector.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>

#include <stdexcept>
#include <iostream>

using namespace Poco::Data::Keywords;
using namespace Poco::Data;

/// @brief Запросы к MySQL базе данных
namespace querries
{
    static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS Posts (\n"
                                           "ID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,\n"
                                           "UserId INT NOT NULL,"
                                           "Post TEXT NOT NULL"
                                           ") ENGINE=InnoDB CHARSET=utf8";
    static const std::string InsertPost = "INSERT INTO Posts(UserId, Post) VALUES(?, ?)";
    static const std::string SelectPostById = "SELECT DISTINCT ID, UserId, Post FROM Posts WHERE ID = ?";
    static const std::string SelectPostByCondition = "SELECT DISTINCT ID, UserId, Post FROM Posts WHERE UserId = ? AND Post = ?";
    static const std::string DeletePost = "DELETE FROM Posts WHERE ID = ?";
    static const std::string SearchPosts = "SELECT DISTINCT ID, UserId, Post FROM Posts "
                                           "WHERE UserId = ? OR Post = ?";
} // namespace querries

PostsTable::PostsTable(std::shared_ptr<Poco::Data::SessionPool> pool) : pool_{pool}
{
    try
    {
        Session sql = pool_->get();
        Transaction transaction{sql};

        Statement statement{sql};
        statement << querries::CreateTable;
        statement.execute();

        transaction.commit();
    }
    catch (const Poco::NotFoundException &e)
    {
        std::cerr << e.displayText() << std::endl;
        throw;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool PostsTable::Insert(PostRow &post, std::string &error)
{
    try
    {
        Session sql = pool_->get();
        Transaction transaction{sql};
        Statement statement{sql};

        statement << querries::InsertPost, use(post.user), use(post.post);

        const size_t res = statement.execute();

        if (res != 1)
        {
            error = messages::InsertionError;
            return false;
        }

        transaction.commit();

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool PostsTable::FindById(size_t id, PostRow &post, std::string &error)
{
    try
    {
        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool PostsTable::Delete(size_t id, std::string &error)
{
    try
    {
        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool PostsTable::FindByCondition(PostRow &condition, PostRow &post, std::string &error)
{
    try
    {
        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

std::shared_ptr<Poco::Data::SessionPool> PostsTable::GetPool()
{
    return pool_;
}
