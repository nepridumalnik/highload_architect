#include <service/database/models/users/user_row.hpp>

#include <service/utils/utils.hpp>

#include <service/resources/jsons.hpp>

#include <nlohmann/json.hpp>

#include <chrono>

bool UserRow::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    for (const auto &field : {std::ref(json_fields::FirstName), std::ref(json_fields::SecondName),
                              std::ref(json_fields::Interests), std::ref(json_fields::City),
                              std::ref(json_fields::Password), std::ref(json_fields::Email)})
    {
        if (!object.contains(field) && !object[field].is_string())
        {
            return false;
        }
    }

    if (!object.contains(json_fields::Age) && !object[json_fields::Age].is_number())
    {
        return false;
    }
    if (!object.contains(json_fields::Male) && !object[json_fields::Male].is_boolean())
    {
        return false;
    }

    id = 0;
    name = object[json_fields::FirstName].get<std::string>();
    secondName = object[json_fields::SecondName].get<std::string>();
    age = object[json_fields::Age].get<size_t>();
    male = object[json_fields::Male].get<bool>();
    interests = object[json_fields::Interests].get<std::string>();
    city = object[json_fields::City].get<std::string>();
    password = object[json_fields::Password].get<std::string>();
    email = object[json_fields::Email].get<std::string>();

    return true;
}

std::string UserRow::ToString() const
{
    return ToJson().dump();
}

nlohmann::json UserRow::ToJson() const
{
    nlohmann::json object;

    object[json_fields::Id] = id;
    object[json_fields::FirstName] = name;
    object[json_fields::SecondName] = secondName;
    object[json_fields::Age] = age;
    object[json_fields::Male] = male;
    object[json_fields::Interests] = interests;
    object[json_fields::City] = city;
    object[json_fields::Password] = password;
    object[json_fields::Email] = email;

    return object;
}

std::string UserRow::Tokenize() const
{
    static const std::string header = nlohmann::json{{"alg", "MD5"}, {"typ", "JWT"}}.dump();
    const std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();
    const std::string timestampStr = std::to_string(timestamp.time_since_epoch().count());
    const std::string payload = nlohmann::json{{json_fields::Password, password}, {json_fields::Email, email}, {json_fields::Timestamp, timestampStr}}.dump();
    const std::string signature = HashMD5(payload);

    std::string token;
    token.reserve(header.size() + payload.size() + signature.size() + timestampStr.size() + 2);

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
