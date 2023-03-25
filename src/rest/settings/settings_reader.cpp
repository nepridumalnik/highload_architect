#include <settings/settings_reader.hpp>

#include <filesystem>
#include <iostream>
#include <fstream>

bool SettingsReader::ReadFile(const std::string &path)
{
    if (!std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
    {
        return false;
    }
    std::ifstream file(path);
    if (!file)
    {
        return false;
    }
    const std::string content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    try
    {
        parameters_ = nlohmann::json::parse(content);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        return false;
    }
    return true;
}
void SettingsReader::Clear()
{
    parameters_.clear();
}