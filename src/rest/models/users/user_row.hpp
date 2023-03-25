#pragma once

#include <models/abstract_table_row.hpp>

/// @brief Пользователь
struct UserRow : public AbstractTableRow
{
    /// @brief Идентификатор
    int id;

    /// @brief Имя
    std::string name;

    /// @brief Фамилия
    std::string secondName;

    /// @brief Возраст
    size_t age;

    /// @brief Пол: true мужчина, иначе женщина
    bool male;

    /// @brief Интересы
    std::string interests;

    /// @brief Город
    std::string city;

    /// @brief Пароль
    std::string password;

    /// @brief Почта
    std::string email;

    /// @see AbstractTableRow
    bool FromJson(const std::string &json) final;

    /// @see AbstractTableRow
    std::string ToString() const final;

    /// @see AbstractTableRow
    nlohmann::json ToJson() const final;

    /// @see AbstractTableRow
    bool Validate() const final;

    /// @brief Получить токен
    /// @return Токен
    std::string Tokenize() const;
};
