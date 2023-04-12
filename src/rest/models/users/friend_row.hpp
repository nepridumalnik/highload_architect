#pragma once

#include <models/abstract_table_row.hpp>

#include <string>

/// @brief Условие поиска пользователя
struct FriendRow : public AbstractTableRow
{
    /// @brief Идентификатор
    int id;

    /// @brief Пользователь
    std::string user;

    /// @brief Друг пользователя
    std::string other;
};
