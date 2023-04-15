#pragma once

#include <models/abstract_table_model.hpp>

#include <models/users/post_row.hpp>

#include <memory>
#include <vector>

namespace Poco::Data
{
    class SessionPool;
} // namespace Poco::Data

/// @brief Класс, управляющий таблицей с пользователями
class PostsTable : public AbstractTableModel<PostRow, PostRow, std::shared_ptr<Poco::Data::SessionPool>>
{
public:
    /// @brief Конструктор
    /// @param pool Пул соединений
    explicit PostsTable(std::shared_ptr<Poco::Data::SessionPool> pool_);

    /// @see AbstractTableModel
    bool Insert(PostRow &friendRow, std::string &error) final;

    /// @see AbstractTableModel
    bool FindById(size_t id, PostRow &friendRow, std::string &error) final;

    /// @see AbstractTableModel
    bool FindByCondition(PostRow &condition, PostRow &friendRow, std::string &error) final;

    /// @see AbstractTableModel
    bool Delete(size_t id, std::string &error) final;

    /// @see AbstractTableModel
    std::shared_ptr<Poco::Data::SessionPool> GetPool() final;

private:
    /// @brief Пул соединений
    std::shared_ptr<Poco::Data::SessionPool> pool_;
};
