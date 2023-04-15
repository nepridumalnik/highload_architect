#include <models/users/friends.hpp>

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
    static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS Friends (\n"
                                           "ID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,\n"
                                           "User INT NOT NULL UNIQUE,"
                                           "Friend INT NOT NULL UNIQUE"
                                           ") ENGINE=InnoDB CHARSET=utf8";
    static const std::string InsertFriend = "INSERT INTO Friends(User, Friend) VALUES(?, ?)";
    static const std::string SelectFriendById = "SELECT DISTINCT ID, User, Friend FROM Friends WHERE ID = ?";
    static const std::string SelectFriendByCondition = "SELECT DISTINCT ID, User, Friend FROM Friends WHERE User = ? AND Friend = ?";
    static const std::string DeleteFriend = "DELETE FROM Friends WHERE ID = ?";
    static const std::string SearchFriends = "SELECT DISTINCT ID, User, Friend FROM Friends "
                                             "WHERE User = ? OR Friend = ?";
} // namespace querries

FriendsTable::FriendsTable(std::shared_ptr<Poco::Data::SessionPool> pool) : pool_{pool}
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

bool FriendsTable::Insert(FriendRow &friendRow, std::string &error)
{
    try
    {
        Session sql = pool_->get();
        Transaction transaction{sql};
        Statement statement{sql};

        statement << querries::InsertFriend, use(friendRow.user), use(friendRow.other);

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

bool FriendsTable::FindById(size_t id, FriendRow &friendRow, std::string &error)
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

bool FriendsTable::Delete(size_t id, std::string &error)
{
    try
    {
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool FriendsTable::FindByCondition(FriendRow &condition, FriendRow &friendRow, std::string &error)
{
    try
    {
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

std::shared_ptr<Poco::Data::SessionPool> FriendsTable::GetPool()
{
    return pool_;
}
