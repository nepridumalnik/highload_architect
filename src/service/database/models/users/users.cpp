#include <service/database/models/users/users.hpp>

#include <soci/transaction.h>
#include <soci/session.h>

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

#include <nlohmann/json.hpp>

#include <stdexcept>
#include <iostream>

using boost::uuids::detail::md5;

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
                                           "Password VARCHAR(50) NOT NULL\n"
                                           ") ENGINE=InnoDB";
    static const std::string InsertUser = "INSERT INTO Users(Name, SecondName, Age, Male, Interests, City, Password) "
                                          "VALUES(:Name, :SecondName, :Age, :Male, :Interests, :City, :Password)";
    static const std::string SelectUser = "SELECT ID, Name, SecondName, Age, Male, Interests, City, Password "
                                          "FROM Users WHERE ID = :ID";
    static const std::string DeleteUser = "DELETE FROM Users WHERE ID = :ID";
} // namespace querries

/// @brief Имена полей для JSON объекте
namespace json_fields
{
    static const std::string id = "id";
    static const std::string first_name = "first_name";
    static const std::string second_name = "second_name";
    static const std::string age = "age";
    static const std::string male = "male";
    static const std::string interests = "interests";
    static const std::string city = "city";
    static const std::string password = "password";
} // json_fields

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
        insert_.exchange(soci::use(user.password));

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

bool UsersTable::FindById(const size_t id, User &user)
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

void UsersTable::Delete(const size_t id)
{
    try
    {
        soci::transaction transaction{*sql_};

        delete_.exchange(soci::use(id));

        delete_.define_and_bind();
        delete_.execute(true);
        delete_.bind_clean_up();

        transaction.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool User::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    if (!object.contains(json_fields::first_name) && !object[json_fields::first_name].is_string())
    {
        return false;
    }
    if (!object.contains(json_fields::second_name) && !object[json_fields::second_name].is_string())
    {
        return false;
    }
    if (!object.contains(json_fields::age) && !object[json_fields::age].is_number())
    {
        return false;
    }
    if (!object.contains(json_fields::male) && !object[json_fields::male].is_boolean())
    {
        return false;
    }
    if (!object.contains(json_fields::interests) && !object[json_fields::interests].is_string())
    {
        return false;
    }
    if (!object.contains(json_fields::city) && !object[json_fields::city].is_string())
    {
        return false;
    }
    if (!object.contains(json_fields::password) && !object[json_fields::password].is_string())
    {
        return false;
    }

    id = 0;
    name = object[json_fields::first_name].get<std::string>();
    secondName = object[json_fields::second_name].get<std::string>();
    age = object[json_fields::age].get<size_t>();
    male = object[json_fields::male].get<bool>();
    interests = object[json_fields::interests].get<std::string>();
    city = object[json_fields::city].get<std::string>();
    password = object[json_fields::password].get<std::string>();

    return true;
}

std::string User::ToJson()
{
    nlohmann::json object;

    object[json_fields::id] = id;
    object[json_fields::first_name] = name;
    object[json_fields::second_name] = secondName;
    object[json_fields::age] = age;
    object[json_fields::male] = male;
    object[json_fields::interests] = interests;
    object[json_fields::city] = city;
    object[json_fields::password] = password;

    return object.dump();
}

std::string User::Tokenize()
{
    static const std::string header = nlohmann::json{{"alg", "MD5"}, {"typ", "JWT"}}.dump();
    const std::string payload = ToJson();
    std::string signature;

    {
        md5 hash;
        md5::digest_type digest;

        hash.process_bytes(payload.data(), payload.size());
        hash.get_digest(digest);

        const int *const intDigest = reinterpret_cast<const int *>(&digest);
        boost::algorithm::hex(intDigest, intDigest + (sizeof(md5::digest_type) / sizeof(int)), std::back_inserter(signature));
    }

    std::string token;
    token.reserve(header.size() + payload.size() + signature.size() + 2);

    token = header + '.';
    token += payload + '.';
    token += signature;

    return token;
}
