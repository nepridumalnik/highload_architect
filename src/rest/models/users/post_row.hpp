#pragma once

#include <models/abstract_table_row.hpp>

#include <string>

/// @brief Условие поиска пользователя
struct PostRow : public AbstractTableRow
{
    /// @brief Идентификатор
    int id;

    /// @brief Пользователь
    int user;

    /// @brief Пост
    std::string post;

    /// @see AbstractTableRow
    bool FromJson(const std::string &json) final;

    /// @see AbstractTableRow
    std::string ToString() const final;

    /// @see AbstractTableRow
    nlohmann::json ToJson() const final;

    /// @see AbstractTableRow
    bool Validate() const final;
};