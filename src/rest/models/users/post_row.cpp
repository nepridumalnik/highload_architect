#include <models/users/post_row.hpp>

#include <utils/utils.hpp>

#include <resources/jsons.hpp>

#include <nlohmann/json.hpp>

bool PostRow::FromJson(const std::string &json)
{
    const nlohmann::json object = nlohmann::json::parse(json);

    if (!object.contains(json_fields::User) || !object[json_fields::User].is_number_integer())
    {
        return false;
    }
    if (!object.contains(json_fields::Post) || !object[json_fields::Post].is_string())
    {
        return false;
    }

    id = 0;
    user = object[json_fields::User].get<int>();
    post = object[json_fields::Post].get<std::string>();

    return true;
}

std::string PostRow::ToString() const
{
    return ToJson().dump();
}

nlohmann::json PostRow::ToJson() const
{
    nlohmann::json object;

    object[json_fields::Id] = id;
    object[json_fields::User] = user;
    object[json_fields::Post] = post;

    return object;
}

bool PostRow::Validate() const
{
    return true;
}
