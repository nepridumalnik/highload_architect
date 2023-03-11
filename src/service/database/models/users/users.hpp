#pragma once

#include <service/database/models/abstract_table_model.hpp>

#include <service/database/models/users/user_row.hpp>

#include <soci/statement.h>

#include <memory>

namespace soci
{
    class session;
} // namespace soci

/// @brief Класс, управляющий таблицей с пользователями
class UsersTable : AbstractTableModel<UserRow>
{
public:
    /// @brief Конструктор
    /// @param sql База данных
    explicit UsersTable(std::shared_ptr<soci::session> sql);

    /// @see AbstractTableModel
    virtual bool Insert(const UserRow &user) final;

    /// @see AbstractTableModel
    virtual bool FindById(const size_t id, UserRow &user) final;

    /// @see AbstractTableModel
    bool Delete(const size_t id) final;

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
