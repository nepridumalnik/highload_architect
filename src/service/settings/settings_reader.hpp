#pragma once

#include <nlohmann/json.hpp>

#include <type_traits>
#include <string>

class SettingsReader
{
public:
    /// @brief Прочитать файл
    /// @param path Путь к файлу
    /// @return True, если успешно
    bool ReadFile(const std::string &path);

    /// @brief Получить параметр
    /// @tparam T Тип данных
    /// @param name Имя параметра
    /// @return Значение параметра или пустую строку
    template <typename T>
    T GetParameter(const std::string &name)
    {
        if (parameters_.contains(name))
        {
            if constexpr (std::is_same_v<bool, T>)
            {
                return parameters_[name].get<T>();
            }
            else if (std::is_same_v<int, T>)
            {
                return parameters_[name].get<T>();
            }
            else if (std::is_same_v<std::string, T>)
            {
                return parameters_[name].get<T>();
            }
        }

        return {};
    }

    /// @brief Очистить настройки
    void Clear();

private:
    nlohmann::json parameters_;
};
