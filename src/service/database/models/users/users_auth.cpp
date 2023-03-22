#include <service/database/models/users/users_auth.hpp>

#include <service/database/models/users/users.hpp>

#include <service/resources/messages.hpp>

#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Transaction.h>
#include <Poco/Data/Statement.h>

#include <stdexcept>
#include <iostream>

using namespace Poco::Data::Keywords;
using namespace Poco::Data;

/// @brief Запросы к MySQL базе данных
namespace querries
{
    static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS Tokens (\n"
                                           "ID INT NOT NULL UNIQUE,\n"
                                           "Token VARCHAR(255) NOT NULL UNIQUE,\n"
                                           "FOREIGN KEY (ID) REFERENCES Users(ID),\n"
                                           "INDEX (Token)\n,"
                                           "INDEX (ID)\n"
                                           ") ENGINE=InnoDB CHARSET=utf8;";
    static const std::string InsertUser = "INSERT INTO Tokens(ID, Token) "
                                          "VALUES(:ID, :Token)";
    static const std::string SelectUserById = "SELECT DISTINCT ID, Token "
                                              "FROM Tokens WHERE ID = :ID";
    static const std::string SelectUserByCondition = "SELECT DISTINCT ID, Token "
                                                     "FROM Tokens WHERE Token = :Token";
    static const std::string DeleteUser = "DELETE FROM Tokens WHERE ID = :ID";
} // namespace querries

UsersAuthTable::UsersAuthTable(std::shared_ptr<Poco::Data::SessionPool> pool)
    : pool_{pool}
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

bool UsersAuthTable::Insert(const UserAuthRow &auth, std::string &error)
{
    try
    {
        if (!auth.Validate())
        {
            return false;
        }

        Session sql = pool_->get();
        Transaction transaction{sql};

        // sql << querries::InsertUser, use(auth.id), use(auth.token);

        // if (st.get_affected_rows() == 0)
        // {
        //     error = messages::InsertionError;
        //     return false;
        // }

        transaction.commit();

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool UsersAuthTable::FindById(const size_t id, UserAuthRow &auth, std::string &error)
{
    try
    {
        Session sql = pool_->get();
        // sql << querries::SelectUserById, use(id),
        //     into(auth.id), into(auth.token);

        // if (st.get_affected_rows() == 0)
        // {
        //     error = messages::NotFound;
        //     return false;
        // }

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool UsersAuthTable::Delete(const size_t id, std::string &error)
{
    try
    {
        Session sql = pool_->get();
        Transaction transaction{sql};
        // sql << querries::DeleteUser, use(id);

        // if (st.get_affected_rows() == 0)
        // {
        //     error = messages::DeletionError;
        //     return false;
        // }

        transaction.commit();

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

bool UsersAuthTable::FindByCondition(const std::string &token, UserAuthRow &auth, std::string &error)
{
    try
    {
        Session sql = pool_->get();
        // sql << querries::SelectUserByCondition,
        //     use(token), into(auth.id), into(auth.token);

        // if (st.get_affected_rows() == 0)
        // {
        //     error = messages::NotFound;
        //     return false;
        // }

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}

std::shared_ptr<Poco::Data::SessionPool> UsersAuthTable::GetPool()
{
    return pool_;
}
