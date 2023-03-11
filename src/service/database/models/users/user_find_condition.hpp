#pragma once

#include <string>

/// @brief Условие поиска пользователя
struct UserRowCond
{
    /// @brief Пароль
    const std::string &password;

    /// @brief Почта
    const std::string &email;
};
