#include <service/database/models/users/user_row.hpp>

#include <service/utils/utils.hpp>

#include <nlohmann/json.hpp>

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
    static const std::string email = "email";
} // json_fields

bool UserRow::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    for (const auto &field : {std::ref(json_fields::first_name), std::ref(json_fields::second_name),
                              std::ref(json_fields::interests), std::ref(json_fields::city),
                              std::ref(json_fields::password), std::ref(json_fields::email)})
    {
        if (!object.contains(field) && !object[field].is_string())
        {
            return false;
        }
    }

    if (!object.contains(json_fields::age) && !object[json_fields::age].is_number())
    {
        return false;
    }
    if (!object.contains(json_fields::male) && !object[json_fields::male].is_boolean())
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
    email = object[json_fields::email].get<std::string>();

    return true;
}

std::string UserRow::ToJson() const
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
    object[json_fields::email] = email;

    return object.dump();
}

std::string UserRow::Tokenize() const
{
    // Задел на будущее, чтобы пока Base64 не тащить
    static const std::string header = nlohmann::json{{"alg", "MD5"}, {"typ", "JWT"}}.dump();
    const std::string payload = nlohmann::json{{json_fields::password, password}, {json_fields::email, email}}.dump();
    const std::string signature = HashMD5(payload);

    std::string token;
    token.reserve(header.size() + payload.size() + signature.size() + 2);

    token = header + '.';
    token += payload + '.';
    token += signature;

    return HashMD5(token);
}

bool UserRow::Validate() const
{
    static const size_t maxVCharLen = 50;

    for (const auto &str : {std::ref(name), std::ref(secondName), std::ref(city),
                            std::ref(password), std::ref(email)})
    {
        const auto &ref = str.get();

        if (ref.empty() || ref.size() > maxVCharLen)
        {
            return false;
        }
    }

    return (std::string::npos != email.find('@'));
}
