#pragma once

#include <string>

struct UserRowCond
{
    /// @brief Пароль
    const std::string &password;

    /// @brief Почта
    const std::string &email;
};
