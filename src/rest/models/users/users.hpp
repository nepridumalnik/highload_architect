#pragma once

#include <models/abstract_table_model.hpp>

#include <models/users/user_row.hpp>
#include <models/users/user_find_condition.hpp>

#include <memory>
#include <vector>

namespace Poco::Data
{
    class SessionPool;
} // namespace Poco::Data

/// @brief Класс, управляющий таблицей с пользователями
class UsersTable : public AbstractTableModel<UserRow, UserRowCond, std::shared_ptr<Poco::Data::SessionPool>>
{
public:
    /// @brief Конструктор
    /// @param pool Пул соединений
    explicit UsersTable(std::shared_ptr<Poco::Data::SessionPool> pool_);

    /// @see AbstractTableModel
    bool Insert(UserRow &user, std::string &error) final;

    /// @see AbstractTableModel
    bool FindById(size_t id, UserRow &user, std::string &error) final;

    /// @see AbstractTableModel
    bool FindByCondition(UserRowCond &condition, UserRow &user, std::string &error) final;

    /// @see AbstractTableModel
    bool Delete(size_t id, std::string &error) final;

    /// @see AbstractTableModel
    std::shared_ptr<Poco::Data::SessionPool> GetPool() final;

    /// @brief Поиск по имени и фамилии
    /// @param firstName Имя
    /// @param secondName Фамилия
    /// @param error Строка с ошибкой
    /// @return True, если удачно
    bool SearchByNames(std::vector<UserRow> &users, std::string &firstName, std::string &secondName, std::string &error);

private:
    /// @brief Пул соединений
    std::shared_ptr<Poco::Data::SessionPool> pool_;
};
