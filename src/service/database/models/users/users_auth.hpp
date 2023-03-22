#pragma once

#include <service/database/models/abstract_table_model.hpp>

#include <service/database/models/users/user_auth_row.hpp>
#include <service/database/models/users/user_find_condition.hpp>

#include <memory>

class UsersTable;

namespace Poco::Data
{
    class SessionPool;
} // namespace Poco::Data

/// @brief Класс, управляющий таблицей с авторизациями пользователей
class UsersAuthTable
    : public AbstractTableModel<UserAuthRow, const std::string &, std::shared_ptr<Poco::Data::SessionPool>>
{
public:
    /// @brief Конструктор
    /// @param pool База данных с пользовательской таблицей, от которой зависит эта таблица
    explicit UsersAuthTable(std::shared_ptr<Poco::Data::SessionPool> pool);

    /// @see AbstractTableModel
    bool Insert(const UserAuthRow &auth, std::string &error) final;

    /// @see AbstractTableModel
    bool FindById(const size_t id, UserAuthRow &auth, std::string &error) final;

    /// @see AbstractTableModel
    bool FindByCondition(const std::string &condition, UserAuthRow &auth, std::string &error) final;

    /// @see AbstractTableModel
    bool Delete(const size_t id, std::string &error) final;

    /// @see AbstractTableModel
    std::shared_ptr<Poco::Data::SessionPool> GetPool() final;

private:
    /// @brief Пул соединений
    std::shared_ptr<Poco::Data::SessionPool> pool_;
};
