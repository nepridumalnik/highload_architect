#include <models/users/friend_row.hpp>

#include <utils/utils.hpp>

#include <resources/jsons.hpp>

#include <nlohmann/json.hpp>

bool FriendRow::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    for (const auto &field : {std::ref(json_fields::Id), std::ref(json_fields::User),
                              std::ref(json_fields::Friend)})
    {
        if (!object.contains(field) && !object[field].is_string())
        {
            return false;
        }
    }

    id = 0;
    user = object[json_fields::User].get<int>();
    other = object[json_fields::Friend].get<int>();

    return true;
}

std::string FriendRow::ToString() const
{
    return ToJson().dump();
}

nlohmann::json FriendRow::ToJson() const
{
    nlohmann::json object;

    object[json_fields::Id] = id;
    object[json_fields::User] = user;
    object[json_fields::Friend] = other;

    return object;
}

bool FriendRow::Validate() const
{
    return true;
}
