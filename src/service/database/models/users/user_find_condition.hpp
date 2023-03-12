#pragma once

#include <service/database/models/abstract_table_row.hpp>

#include <string>

/// @brief Условие поиска пользователя
struct UserRowCond : public AbstractTableRow
{
    /// @brief Пароль
    std::string password;

    /// @brief Почта
    std::string email;

    /// @see AbstractTableRow
    bool FromJson(const std::string &json) final;

    /// @see AbstractTableRow
    std::string ToJson() const final;

    /// @see AbstractTableRow
    bool Validate() const final;
};
