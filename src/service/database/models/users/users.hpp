#pragma once

#include <service/database/models/abstract_table_model.hpp>

#include <service/database/models/users/user_row.hpp>
#include <service/database/models/users/user_find_condition.hpp>

#include <memory>
#include <vector>

namespace soci
{
    class session;
} // namespace soci

/// @brief Класс, управляющий таблицей с пользователями
class UsersTable : public AbstractTableModel<UserRow, UserRowCond, std::shared_ptr<soci::session>>
{
public:
    /// @brief Конструктор
    /// @param sql База данных
    explicit UsersTable(std::shared_ptr<soci::session> sql);

    /// @see AbstractTableModel
    bool Insert(const UserRow &user, std::string &error) final;

    /// @see AbstractTableModel
    bool FindById(const size_t id, UserRow &user, std::string &error) final;

    /// @see AbstractTableModel
    bool FindByCondition(const UserRowCond &condition, UserRow &user, std::string &error) final;

    /// @see AbstractTableModel
    bool Delete(const size_t id, std::string &error) final;

    /// @see AbstractTableModel
    std::shared_ptr<soci::session> GetDatabase() final;

    /// @brief Поиск по имени и фамилии
    /// @param firstName Имя
    /// @param secondName Фамилия
    /// @param error Строка с ошибкой
    /// @return True, если удачно
    bool SearchByNames(std::vector<UserRow> &users, const std::string &firstName, const std::string &secondName, std::string &error);

private:
    /// @brief База данных
    std::shared_ptr<soci::session> sql_;
};
