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
    static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS Users (\n"
                                           "ID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,\n"
                                           "Name VARCHAR(50) NOT NULL,\n"
                                           "SecondName VARCHAR(50) NOT NULL,\n"
                                           "Age INT NOT NULL,\n"
                                           "Male BOOLEAN NOT NULL,\n"
                                           "Interests TEXT NOT NULL,\n"
                                           "City VARCHAR(50) NOT NULL,\n"
                                           "Password VARCHAR(50) NOT NULL,\n"
                                           "Email VARCHAR(50) NOT NULL UNIQUE,\n"
                                           "INDEX (Name, Email)\n"
                                           ") ENGINE=InnoDB CHARSET=utf8";
    static const std::string InsertUser = "INSERT INTO Users(Name, SecondName, Age, Male, Interests, City, Password, Email) "
                                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
    static const std::string SelectUserById = "SELECT DISTINCT ID, Name, SecondName, Age, Male, Interests, City, Password, Email "
                                              "FROM Users WHERE ID = ?";
    static const std::string SelectUserByCondition = "SELECT DISTINCT ID, Name, SecondName, Age, Male, Interests, City, Password, Email "
                                                     "FROM Users WHERE Password = ? AND Email = ?";
    static const std::string DeleteUser = "DELETE FROM Users WHERE ID = ?";
    static const std::string SearchUsers = "SELECT ID, Name, SecondName, Age, Male, Interests, City, Password, Email "
                                           "FROM Users WHERE Name LIKE ? AND SecondName LIKE ?;";
} // namespace querries

FriendsTable::FriendsTable(std::shared_ptr<Poco::Data::SessionPool> pool) : pool_{pool}
{
    try
    {
        // Session sql = pool_->get();
        // Transaction transaction{sql};
        // sql << querries::CreateTable;
        // transaction.commit();
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
        // if (!user.Validate())
        // {
        //     return false;
        // }

        // Session sql = pool_->get();
        // Transaction transaction{sql};
        // Statement statement{sql};

        // std::string tmpPassword = HashMD5(user.password);

        // statement << querries::InsertUser,
        //     use(user.name), use(user.secondName),
        //     use(user.age), into(*reinterpret_cast<int *>(&user.male)),
        //     use(user.interests), use(user.city),
        //     use(tmpPassword), use(user.email);

        // const size_t res = statement.execute();

        // if (res != 1)
        // {
        //     error = messages::InsertionError;
        //     return false;
        // }

        // transaction.commit();

        // return true;
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
        // Session sql = pool_->get();
        // Statement statement{sql};

        // statement << querries::SelectUserById, use(id),
        //     into(user.id), into(user.name),
        //     into(user.secondName), into(user.age),
        //     into(*(reinterpret_cast<int *>(&user.male))), into(user.interests),
        //     into(user.city), into(user.password), into(user.email);

        // const size_t res = statement.execute();

        // if (res != 1)
        // {
        //     error = messages::NotFound;
        //     return false;
        // }

        // return true;
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
        // Session sql = pool_->get();
        // Transaction transaction{sql};
        // Statement statement{sql};
        // statement << querries::DeleteUser, use(id);

        // const size_t res = statement.execute();

        // if (res != 1)
        // {
        //     error = messages::DeletionError;
        //     return false;
        // }

        // transaction.commit();

        // return true;
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
        // std::string tmpPassword = HashMD5(condition.password);
        // Session sql = pool_->get();
        // Statement statement{sql};
        // statement << querries::SelectUserByCondition,
        //     use(tmpPassword), use(condition.email),
        //     into(user.id), into(user.name), into(user.secondName),
        //     into(user.age), into(*reinterpret_cast<int *>(&user.male)),
        //     into(user.interests), into(user.city), into(user.password),
        //     into(user.email);

        // const size_t res = statement.execute();

        // if (res != 1)
        // {
        //     error = messages::NotFound;
        //     return false;
        // }

        // return true;
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
