#include <service/database/models/users/users.hpp>

#include <service/resources/messages.hpp>

#include <service/utils/utils.hpp>

#include <soci/transaction.h>
#include <soci/statement.h>
#include <soci/session.h>
#include <soci/row-exchange.h>
#include <soci/rowset.h>
#include <soci/row.h>

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
                                           "Email VARCHAR(50) NOT NULL UNIQUE,\n"
                                           "INDEX (Name, Email)\n"
                                           ") ENGINE=InnoDB CHARSET=utf8";
    static const std::string InsertUser = "INSERT INTO Users(Name, SecondName, Age, Male, Interests, City, Password, Email) "
                                          "VALUES(:Name, :SecondName, :Age, :Male, :Interests, :City, :Password, :Email)";
    static const std::string SelectUserById = "SELECT DISTINCT ID, Name, SecondName, Age, Male, Interests, City, Password, Email "
                                              "FROM Users WHERE ID = :ID";
    static const std::string SelectUserByCondition = "SELECT DISTINCT ID, Name, SecondName, Age, Male, Interests, City, Password, Email "
                                                     "FROM Users WHERE Password = :Password AND Email = :Email";
    static const std::string DeleteUser = "DELETE FROM Users WHERE ID = :ID";
    static const std::string SearchUsers = "SELECT ID, Name, SecondName, Age, Male, Interests, City, Password, Email "
                                           "FROM Users WHERE Name LIKE :Name AND SecondName LIKE :SecondName;";
} // namespace querries

UsersTable::UsersTable(std::shared_ptr<soci::session> sql) : sql_{sql}
{
    try
    {
        soci::transaction transaction{*sql_};
        *sql_ << querries::CreateTable;
        transaction.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool UsersTable::Insert(const UserRow &user, std::string &error)
{
    try
    {
        if (!user.Validate())
        {
            return false;
        }

        soci::transaction transaction{*sql_};

        const std::string tmpPassword = HashMD5(user.password);

        soci::statement st = (sql_->prepare << querries::InsertUser,
                              soci::use(user.name), soci::use(user.secondName),
                              soci::use(user.age), soci::use(static_cast<int>(user.male)),
                              soci::use(user.interests), soci::use(user.city),
                              soci::use(tmpPassword), soci::use(user.email));

        st.execute();

        if (st.get_affected_rows() == 0)
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

bool UsersTable::FindById(const size_t id, UserRow &user, std::string &error)
{
    try
    {
        soci::statement st = (sql_->prepare << querries::SelectUserById, soci::use(id),
                              soci::into(user.id), soci::into(user.name),
                              soci::into(user.secondName), soci::into(user.age),
                              soci::into(*reinterpret_cast<int *>(&user.male)), soci::into(user.interests),
                              soci::into(user.city), soci::into(user.password), soci::into(user.email));

        st.execute(true);

        if (st.get_affected_rows() == 0)
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

bool UsersTable::Delete(const size_t id, std::string &error)
{
    try
    {
        soci::transaction transaction{*sql_};
        soci::statement st = (sql_->prepare << querries::DeleteUser, soci::use(id));

        st.execute();

        if (st.get_affected_rows() == 0)
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

bool UsersTable::FindByCondition(const UserRowCond &condition, UserRow &user, std::string &error)
{
    try
    {
        const std::string tmpPassword = HashMD5(condition.password);
        soci::statement st = (sql_->prepare << querries::SelectUserByCondition,
                              soci::use(tmpPassword), soci::use(condition.email),
                              soci::into(user.id), soci::into(user.name), soci::into(user.secondName),
                              soci::into(user.age), soci::into(*reinterpret_cast<int *>(&user.male)),
                              soci::into(user.interests), soci::into(user.city), soci::into(user.password),
                              soci::into(user.email));

        st.execute(true);

        if (st.get_affected_rows() == 0)
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

std::shared_ptr<soci::session> UsersTable::GetDatabase()
{
    return sql_;
}

bool UsersTable::SearchByNames(std::vector<UserRow> &users, const std::string &firstName, const std::string &secondName, std::string &error)
{
    try
    {
        soci::rowset<soci::row> rowset = (sql_->prepare << querries::SearchUsers,
                                          soci::use(firstName), soci::use(secondName));
        soci::row row;
        soci::statement st = (sql_->prepare << querries::SearchUsers,
                              soci::use(firstName), soci::use(secondName));

        st.exchange_for_rowset(soci::into(row));
        st.execute(false);

        const size_t distance = std::distance(rowset.begin(), rowset.end());
        users.clear();
        users.resize(distance);

        soci::rowset_iterator<soci::row> it(st, row);
        soci::rowset_iterator<soci::row> end;
        size_t counter = 0;

        for (; it != end; ++it)
        {
            users[counter].name = it->get<std::string>(1);
            users[counter].id = it->get<int>(0);
            users[counter].secondName = it->get<std::string>(2);
            users[counter].age = it->get<int>(3);
            users[counter].male = it->get<int>(4);
            users[counter].interests = it->get<std::string>(5);
            users[counter].city = it->get<std::string>(5);
            users[counter].password = it->get<std::string>(6);
            users[counter].email = it->get<std::string>(7);
            ++counter;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        error = e.what();
    }

    return false;
}
