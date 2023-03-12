#pragma once

#include <service/database/models/abstract_table_model.hpp>

#include <service/database/models/users/user_auth_row.hpp>
#include <service/database/models/users/user_find_condition.hpp>

#include <memory>

class UsersTable;

namespace soci
{
    class session;
} // namespace soci

/// @brief Класс, управляющий таблицей с авторизациями пользователей
class UsersAuthTable : public AbstractTableModel<UserAuthRow, const std::string &, std::shared_ptr<soci::session>>
{
public:
    /// @brief Конструктор
    /// @param sql База данных с пользовательской таблицей, от которой зависит эта таблица
    explicit UsersAuthTable(std::shared_ptr<UsersTable> userTable);

    /// @see AbstractTableModel
    bool Insert(const UserAuthRow &auth) final;

    /// @see AbstractTableModel
    bool FindById(const size_t id, UserAuthRow &auth) final;

    /// @see AbstractTableModel
    bool FindByCondition(const std::string &condition, UserAuthRow &auth) final;

    /// @see AbstractTableModel
    bool Delete(const size_t id) final;

    /// @see AbstractTableModel
    std::shared_ptr<soci::session> GetDatabase() final;

private:
    /// @brief База данных
    std::shared_ptr<soci::session> sql_;
};
