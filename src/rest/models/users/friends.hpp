#pragma once

#include <models/abstract_table_model.hpp>

#include <models/users/friend_row.hpp>

#include <memory>
#include <vector>

namespace Poco::Data
{
    class SessionPool;
} // namespace Poco::Data

/// @brief Класс, управляющий таблицей с пользователями
class FriendsTable : public AbstractTableModel<FriendRow, FriendRow, std::shared_ptr<Poco::Data::SessionPool>>
{
public:
    /// @brief Конструктор
    /// @param pool Пул соединений
    explicit FriendsTable(std::shared_ptr<Poco::Data::SessionPool> pool_);

    /// @see AbstractTableModel
    bool Insert(FriendRow &friendRow, std::string &error) final;

    /// @see AbstractTableModel
    bool FindById(size_t id, FriendRow &friendRow, std::string &error) final;

    /// @see AbstractTableModel
    bool FindByCondition(FriendRow &condition, FriendRow &friendRow, std::string &error) final;

    /// @see AbstractTableModel
    bool Delete(size_t id, std::string &error) final;

    /// @see AbstractTableModel
    std::shared_ptr<Poco::Data::SessionPool> GetPool() final;

private:
    /// @brief Пул соединений
    std::shared_ptr<Poco::Data::SessionPool> pool_;
};
