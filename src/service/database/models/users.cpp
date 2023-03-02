#include <service/database/models/users.hpp>

#include <soci/transaction.h>
#include <soci/session.h>

#include <stdexcept>
#include <iostream>

namespace querries
{

    /// @brief Создать таблицу пользователей
    static const std::string CreateTable = "CREATE TABLE IF NOT EXISTS Users (\n"
                                           "ID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,\n"
                                           "Name VARCHAR(50) NOT NULL,\n"
                                           "SecondName VARCHAR(50) NOT NULL,\n"
                                           "Age INT NOT NULL,\n"
                                           "Male BOOLEAN NOT NULL,\n"
                                           "Interests TEXT NOT NULL,\n"
                                           "City VARCHAR(50) NOT NULL\n"
                                           ") ENGINE=InnoDB";

    /// @brief Добавить пользователя
    static const std::string InsertUser = "INSERT INTO Users(Name, SecondName, Age, Male, Interests, City) "
                                          "VALUES(:Name, :SecondName, :Age, :Male, :Interests, :City)";

    /// @brief Выбрать пользователя
    static const std::string SelectUser = "SELECT ID, Name, SecondName, Age, Male, Interests, City "
                                          "FROM Users WHERE ID = :ID";

} // namespace querries

UsersTable::UsersTable(std::shared_ptr<soci::session> sql)
    : sql_{sql},
      insert_{*sql_},
      select_{*sql_}
{
    try
    {
        soci::transaction transaction{*sql_};
        *sql_ << querries::CreateTable;
        transaction.commit();

        // Компиляция команд
        insert_.prepare(querries::InsertUser);
        select_.prepare(querries::SelectUser);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

void UsersTable::Insert(const User &user)
{
    try
    {
        soci::transaction transaction{*sql_};

        insert_.exchange(soci::use(user.name));
        insert_.exchange(soci::use(user.secondName));
        insert_.exchange(soci::use(user.age));
        insert_.exchange(soci::use(static_cast<int>(user.male)));
        insert_.exchange(soci::use(user.interests));
        insert_.exchange(soci::use(user.city));

        insert_.define_and_bind();
        insert_.execute(true);
        insert_.bind_clean_up();

        transaction.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool UsersTable::FindById(int id, User &user)
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
