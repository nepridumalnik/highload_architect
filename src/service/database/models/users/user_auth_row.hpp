#pragma once

#include <service/database/models/abstract_table_row.hpp>

/// @brief Данные авторизации пользователя
struct UserAuthRow : public AbstractTableRow
{
    /// @brief Идентификатор
    int id;

    /// @brief Пароль
    std::string password;

    /// @brief Почта
    std::string email;

    /// @brief Токен
    std::string token;

    /// @see AbstractTableRow
    bool FromJson(const std::string &json) final;

    /// @see AbstractTableRow
    std::string ToJson() const final;

    /// @see AbstractTableRow
    bool Validate() const final;
};
