#include <service/database/models/users/users.hpp>

#include <soci/transaction.h>
#include <soci/session.h>

#include <stdexcept>
#include <iostream>

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
                                           "Email VARCHAR(50) NOT NULL UNIQUE\n"
                                           ") ENGINE=InnoDB";
    static const std::string InsertUser = "INSERT INTO Users(Name, SecondName, Age, Male, Interests, City, Password, Email) "
                                          "VALUES(:Name, :SecondName, :Age, :Male, :Interests, :City, :Password, :Email)";
    static const std::string SelectUser = "SELECT ID, Name, SecondName, Age, Male, Interests, City, Password, Email "
                                          "FROM Users WHERE ID = :ID";
    static const std::string DeleteUser = "DELETE FROM Users WHERE ID = :ID";
} // namespace querries

UsersTable::UsersTable(std::shared_ptr<soci::session> sql)
    : sql_{sql},
      insert_{*sql_},
      select_{*sql_},
      delete_{*sql_}
{
    try
    {
        soci::transaction transaction{*sql_};
        *sql_ << querries::CreateTable;
        transaction.commit();

        // Компиляция команд
        insert_.prepare(querries::InsertUser);
        select_.prepare(querries::SelectUser);
        delete_.prepare(querries::DeleteUser);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool UsersTable::Insert(const UserRow &user)
{
    try
    {
        if (!user.Validate())
        {
            return false;
        }

        soci::transaction transaction{*sql_};

        insert_.exchange(soci::use(user.name));
        insert_.exchange(soci::use(user.secondName));
        insert_.exchange(soci::use(user.age));
        insert_.exchange(soci::use(static_cast<int>(user.male)));
        insert_.exchange(soci::use(user.interests));
        insert_.exchange(soci::use(user.city));
        insert_.exchange(soci::use(user.password));
        insert_.exchange(soci::use(user.email));

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

bool UsersTable::FindById(const size_t id, UserRow &user)
{
    try
    {
        select_.exchange(soci::use(id));

        select_.exchange(soci::into(user.id));
        select_.exchange(soci::into(user.name));
        select_.exchange(soci::into(user.secondName));
        select_.exchange(soci::into(user.age));
        select_.exchange(soci::into(*reinterpret_cast<int *>(&user.male)));
        select_.exchange(soci::into(user.interests));
        select_.exchange(soci::into(user.city));
        select_.exchange(soci::into(user.password));
        select_.exchange(soci::into(user.email));

        select_.define_and_bind();
        const bool result = select_.execute(true);
        select_.bind_clean_up();

        return result;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool UsersTable::Delete(const size_t id)
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
