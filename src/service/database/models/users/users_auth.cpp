#include <service/database/models/users/users_auth.hpp>

#include <soci/transaction.h>
#include <soci/session.h>

#include <stdexcept>
#include <iostream>

/// @brief Запросы к MySQL базе данных
namespace querries
{
    static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS AuthorizedUsers (\n"
                                           "ID INT NOT NULL PRIMARY KEY,\n"
                                           "Password VARCHAR(50) NOT NULL,\n"
                                           "Email VARCHAR(50) NOT NULL,\n"
                                           "Token VARCHAR(255) NOT NULL\n"
                                           ") ENGINE=InnoDB CHARSET=utf8;";
    static const std::string InsertUser = "INSERT INTO AuthorizedUsers(ID, Password, Email, Token) "
                                          "VALUES(:ID, :Password, :Email, :Token)";
    static const std::string SelectUserById = "SELECT DISTINCT ID, Password, Email, Token "
                                              "FROM AuthorizedUsers WHERE ID = :ID";
    static const std::string SelectUserByCondition = "SELECT DISTINCT ID, Password, Email, Token "
                                                     "FROM AuthorizedUsers WHERE Password = :Password AND Email = :Email";
    static const std::string DeleteUser = "DELETE FROM AuthorizedUsers WHERE ID = :ID";
} // namespace querries

UsersAuthTable::UsersAuthTable(std::shared_ptr<soci::session> sql)
    : sql_{sql},
      insert_{*sql_},
      selectById_{*sql_},
      selectByCondition_{*sql_},
      delete_{*sql_}
{
    try
    {
        {
            soci::transaction transaction{*sql_};
            *sql_ << querries::CreateTable;
            transaction.commit();
        }

        insert_.prepare(querries::InsertUser);
        selectById_.prepare(querries::SelectUserById);
        selectByCondition_.prepare(querries::SelectUserByCondition);
        delete_.prepare(querries::DeleteUser);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool UsersAuthTable::Insert(const UserAuthRow &auth)
{
    try
    {
        if (!auth.Validate())
        {
            return false;
        }

        soci::transaction transaction{*sql_};

        insert_.exchange(soci::use(auth.id));
        insert_.exchange(soci::use(auth.password));
        insert_.exchange(soci::use(auth.email));
        insert_.exchange(soci::use(auth.token));

        insert_.define_and_bind();
        insert_.execute(true);
        insert_.bind_clean_up();

        transaction.commit();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return false;
}

bool UsersAuthTable::FindById(const size_t id, UserAuthRow &auth)
{
    try
    {
        selectById_.exchange(soci::use(id));

        selectById_.exchange(soci::into(auth.id));
        selectById_.exchange(soci::into(auth.password));
        selectById_.exchange(soci::into(auth.email));
        selectById_.exchange(soci::into(auth.token));

        selectById_.define_and_bind();
        const bool result = selectById_.execute(true);
        selectById_.bind_clean_up();

        return result;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return false;
}

bool UsersAuthTable::Delete(const size_t id)
{
    try
    {
        soci::transaction transaction{*sql_};

        delete_.exchange(soci::use(id));

        delete_.define_and_bind();
        delete_.execute(true);
        delete_.bind_clean_up();

        transaction.commit();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return false;
}

bool UsersAuthTable::FindByCondition(const UserRowCond &condition, UserAuthRow &auth)
{
    try
    {
        selectByCondition_.exchange(soci::use(condition.password));
        selectByCondition_.exchange(soci::use(condition.email));

        selectByCondition_.exchange(soci::into(auth.id));
        selectByCondition_.exchange(soci::into(auth.password));
        selectByCondition_.exchange(soci::into(auth.email));
        selectByCondition_.exchange(soci::into(auth.token));

        selectByCondition_.define_and_bind();
        const bool result = selectByCondition_.execute(true);
        selectByCondition_.bind_clean_up();

        return result;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return false;
}
