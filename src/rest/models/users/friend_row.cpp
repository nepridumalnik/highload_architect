#include <models/users/friend_row.hpp>

#include <utils/utils.hpp>

#include <resources/jsons.hpp>

#include <nlohmann/json.hpp>

bool FriendRow::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    if (!object.contains(json_fields::User) || !object[json_fields::User].is_number_integer())
    {
        return false;
    }
    if (!object.contains(json_fields::Friend) || !object[json_fields::Friend].is_number_integer())
    {
        return false;
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
