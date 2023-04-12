#include <models/users/friends.hpp>

#include <utils/utils.hpp>

#include <resources/messages.hpp>
#include <resources/jsons.hpp>

#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Transaction.h>
#include <Poco/Data/Connector.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/MySQL/MySQLException.h>

#include <stdexcept>
#include <iostream>

using namespace Poco::Data::Keywords;
using namespace Poco::Data;

/// @brief Запросы к MySQL базе данных
namespace querries
{
    static const std::string CreateTable = "ENGINE=InnoDB CHARSET=utf8";
    static const std::string InsertFriend = "";
    static const std::string SelectFriendById = "";
    static const std::string SelectFriendByCondition = "";
    static const std::string DeleteFriend = "";
    static const std::string SearchFriends = "";
} // namespace querries

FriendsTable::FriendsTable(std::shared_ptr<Poco::Data::SessionPool> pool) : pool_{pool}
{
    try
    {
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
