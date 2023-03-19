#include <service/database/models/users/user_auth_row.hpp>

#include <service/resources/jsons.hpp>

#include <nlohmann/json.hpp>

bool UserAuthRow::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    for (const auto &field : {std::ref(json_fields::Id), std::ref(json_fields::Token)})
    {
        if (!object.contains(field) && !object[field].is_string())
        {
            return false;
        }
    }

    id = object[json_fields::Id].get<size_t>();
    token = object[json_fields::Token].get<std::string>();

    return true;
}

std::string UserAuthRow::ToString() const
{
    return ToJson().dump();
}

nlohmann::json UserAuthRow::ToJson() const
{
    nlohmann::json object;

    object[json_fields::Id] = id;
    object[json_fields::Token] = token;

    return object;
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
