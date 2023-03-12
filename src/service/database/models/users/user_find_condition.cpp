#include <service/database/models/users/user_find_condition.hpp>

#include <nlohmann/json.hpp>

/// @brief Имена полей для JSON объекте
namespace json_fields
{
    static const std::string Password = "password";
    static const std::string Email = "email";
} // json_fields

bool UserRowCond::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    for (const auto &field : {std::ref(json_fields::Password), std::ref(json_fields::Email)})
    {
        if (!object.contains(field) && !object[field].is_string())
        {
            return false;
        }
    }

    password = object[json_fields::Password].get<std::string>();
    email = object[json_fields::Email].get<std::string>();

    return true;
}

std::string UserRowCond::ToJson() const
{
    nlohmann::json object;

    object[json_fields::Password] = password;
    object[json_fields::Email] = email;

    return object.dump();
}

bool UserRowCond::Validate() const
{
    static const size_t maxVCharLen = 50;

    for (const auto &str : {std::ref(password), std::ref(email)})
    {
        const auto &ref = str.get();

        if (ref.empty() || ref.size() > maxVCharLen)
        {
            return false;
        }
    }

    return (std::string::npos != email.find('@'));
}
