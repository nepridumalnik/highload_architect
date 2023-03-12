#include <service/database/models/users/user_auth_row.hpp>

#include <nlohmann/json.hpp>

/// @brief Имена полей для JSON объекте
namespace json_fields
{
    static const std::string id = "id";
    static const std::string token = "token";
} // json_fields

bool UserAuthRow::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    for (const auto &field : {std::ref(json_fields::id), std::ref(json_fields::token)})
    {
        if (!object.contains(field) && !object[field].is_string())
        {
            return false;
        }
    }

    id = object[json_fields::id].get<size_t>();
    token = object[json_fields::token].get<std::string>();

    return true;
}

std::string UserAuthRow::ToJson() const
{
    nlohmann::json object;

    object[json_fields::id] = id;
    object[json_fields::token] = token;

    return object.dump();
}

bool UserAuthRow::Validate() const
{
    static const size_t maxTokenLen = 255;

    if (token.empty() || token.size() > maxTokenLen)
    {
        return false;
    }

    return true;
}
