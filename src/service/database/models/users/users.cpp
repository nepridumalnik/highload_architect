#include <service/database/models/users/users.hpp>

#include <service/resources/messages.hpp>

#include <service/utils/utils.hpp>

#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Transaction.h>
#include <Poco/Data/Connector.h>
#include <Poco/Data/Statement.h>

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

UsersTable::UsersTable(std::shared_ptr<Poco::Data::SessionPool> pool) : pool_{pool}
{
    try
    {
        Session sql = pool_->get();
        Transaction transaction{sql};
        sql << querries::CreateTable;
        transaction.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool UsersTable::Insert(UserRow &user, std::string &error)
{
    try
    {
        if (!user.Validate())
        {
            return false;
        }

        Session sql = pool_->get();
        Transaction transaction{sql};
        Statement statement{sql};

        std::string tmpPassword = HashMD5(user.password);
        int male = static_cast<int>(user.male);

        statement << querries::InsertUser,
            use(user.name), use(user.secondName),
            use(user.age), use(male),
            use(user.interests), use(user.city),
            use(tmpPassword), use(user.email);

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

bool UsersTable::FindById(size_t id, UserRow &user, std::string &error)
{
    try
    {
        Session sql = pool_->get();
        Statement statement{sql};

        statement << querries::SelectUserById, use(id),
            into(user.id), into(user.name),
            into(user.secondName), into(user.age),
            into(*(reinterpret_cast<int *>(&user.male))), into(user.interests),
            into(user.city), into(user.password), into(user.email);

        const size_t res = statement.execute();

        if (res != 1)
        {
            error = messages::NotFound;
            return false;
        }

        return true;
    }
    catch (const Poco::Data::MySQL::MySQLException &e)
    {
        error = e.displayText();
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool UsersTable::Delete(size_t id, std::string &error)
{
    try
    {
        Session sql = pool_->get();
        Transaction transaction{sql};
        Statement statement{sql};
        statement << querries::DeleteUser, use(id);

        const size_t res = statement.execute();

        if (res != 1)
        {
            error = messages::DeletionError;
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

bool UsersTable::FindByCondition(UserRowCond &condition, UserRow &user, std::string &error)
{
    try
    {
        std::string tmpPassword = HashMD5(condition.password);
        Session sql = pool_->get();
        Statement statement{sql};
        statement << querries::SelectUserByCondition,
            use(tmpPassword), use(condition.email),
            into(user.id), into(user.name), into(user.secondName),
            into(user.age), into(*reinterpret_cast<int *>(&user.male)),
            into(user.interests), into(user.city), into(user.password),
            into(user.email);

        const size_t res = statement.execute();

        if (res != 1)
        {
            error = messages::NotFound;
            return false;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

std::shared_ptr<Poco::Data::SessionPool> UsersTable::GetPool()
{
    return pool_;
}

bool UsersTable::SearchByNames(std::vector<UserRow> &users, const std::string &firstName, const std::string &secondName, std::string &error)
{
    try
    {
        // Session sql = pool_->get();
        // soci::rowset<soci::row> rowset = (sql.prepare << querries::SearchUsers,
        //                                   use(firstName), use(secondName));
        // soci::row row;
        // soci::statement st = (sql.prepare << querries::SearchUsers,
        //                       use(firstName), use(secondName));

        // st.exchange_for_rowset(into(row));
        // st.execute(false);

        // const size_t distance = std::distance(rowset.begin(), rowset.end());
        // users.clear();
        // users.resize(distance);

        // soci::rowset_iterator<soci::row> it(st, row);
        // soci::rowset_iterator<soci::row> end;
        // size_t counter = 0;

        // for (; it != end; ++it)
        // {
        //     users[counter].name = it->get<std::string>(1);
        //     users[counter].id = it->get<int>(0);
        //     users[counter].secondName = it->get<std::string>(2);
        //     users[counter].age = it->get<int>(3);
        //     users[counter].male = it->get<int>(4);
        //     users[counter].interests = it->get<std::string>(5);
        //     users[counter].city = it->get<std::string>(6);
        //     users[counter].password = it->get<std::string>(7);
        //     users[counter].email = it->get<std::string>(8);
        //     ++counter;
        // }

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}
