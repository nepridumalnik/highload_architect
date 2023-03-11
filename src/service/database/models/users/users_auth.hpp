#pragma once

#include <service/database/models/abstract_table_model.hpp>

#include <service/database/models/users/user_auth_row.hpp>
#include <service/database/models/users/user_find_condition.hpp>

#include <soci/statement.h>

#include <memory>

namespace soci
{
    class session;
} // namespace soci

/// @brief Класс, управляющий таблицей с авторизациями пользователей
class UsersAuthTable : public AbstractTableModel<UserAuthRow, UserRowCond>
{
public:
    /// @brief Конструктор
    /// @param sql База данных
    explicit UsersAuthTable(std::shared_ptr<soci::session> sql);

    /// @see AbstractTableModel
    bool Insert(const UserAuthRow &auth) final;

    /// @see AbstractTableModel
    bool FindById(const size_t id, UserAuthRow &auth) final;

    /// @see AbstractTableModel
    bool FindByCondition(const UserRowCond &condition, UserAuthRow &auth) final;

    /// @see AbstractTableModel
    bool Delete(const size_t id) final;

private:
    /// @brief База данных
    std::shared_ptr<soci::session> sql_;

    /// @brief Запрос по добавлению пользователя
    soci::statement insert_;

    /// @brief Запрос по выбору пользователя по идентификатору
    soci::statement selectById_;

    /// @brief Запрос по выбору пользователя по паролю и почте
    soci::statement selectByCondition_;

    /// @brief Запрос по удалению пользователя
    soci::statement delete_;
};
