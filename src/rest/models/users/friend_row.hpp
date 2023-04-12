#pragma once

#include <models/abstract_table_row.hpp>

#include <string>

/// @brief Условие поиска пользователя
struct FriendRow : public AbstractTableRow
{
    /// @brief Идентификатор
    int id;

    /// @brief Пользователь
    int user;

    /// @brief Друг пользователя
    int other;
};
