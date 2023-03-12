#include <service/database/models/users/users_auth.hpp>

#include <service/database/models/users/users.hpp>

#include <soci/transaction.h>
#include <soci/session.h>

#include <stdexcept>
#include <iostream>

/// @brief Запросы к MySQL базе данных
namespace querries
{
    static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS Tokens (\n"
                                           "ID INT NOT NULL,\n"
                                           "Token VARCHAR(255) NOT NULL UNIQUE,\n"
                                           "FOREIGN KEY (ID) REFERENCES Users(ID),\n"
                                           "INDEX (Token)\n,"
                                           "INDEX (ID)\n"
                                           ") ENGINE=InnoDB CHARSET=utf8;";
    static const std::string InsertUser = "INSERT INTO Tokens(ID, Token) "
                                          "VALUES(:ID, :Password, :Email, :Token)";
    static const std::string SelectUserById = "SELECT DISTINCT ID, Token "
                                              "FROM Tokens WHERE ID = :ID";
    static const std::string SelectUserByCondition = "SELECT DISTINCT ID, Token "
                                                     "FROM Tokens WHERE Token = :Token";
    static const std::string DeleteUser = "DELETE FROM Tokens WHERE ID = :ID";
} // namespace querries

UsersAuthTable::UsersAuthTable(std::shared_ptr<UsersTable> userTable)
    : sql_{userTable->GetDatabase()},
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

bool UsersAuthTable::FindByCondition(const std::string &token, UserAuthRow &auth)
{
    try
    {
        selectByCondition_.exchange(soci::use(token));

        selectByCondition_.exchange(soci::into(auth.id));
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

std::shared_ptr<soci::session> UsersAuthTable::GetDatabase()
{
    return sql_;
}
