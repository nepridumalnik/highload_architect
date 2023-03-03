#pragma once

#include <service/database/models/abstract_table_model.hpp>

#include <soci/statement.h>

#include <memory>
#include <string>

namespace soci
{
    class session;
} // namespace soci

/// @brief Пользователь
struct User
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
};

/// @brief Класс, управляющий таблицей с пользователями
class UsersTable : AbstractTableModel<User>
{
public:
    /// @brief Конструктор
    /// @param sql База данных
    explicit UsersTable(std::shared_ptr<soci::session> sql);

    /// @see AbstractTableModel
    virtual void Insert(const User &user) final;

    /// @see AbstractTableModel
    virtual bool FindById(const int id, User &user) final;

    /// @see AbstractTableModel
    void Delete(const int id) final;

private:
    /// @brief База данных
    std::shared_ptr<soci::session> sql_;

    /// @brief Запрос по добавлению пользователя
    soci::statement insert_;

    /// @brief Запрос по выбору одного пользователя
    soci::statement select_;

    /// @brief Запрос по удалению пользователя
    soci::statement delete_;
};
