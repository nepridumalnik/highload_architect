#pragma once

#include <service/database/models/users.hpp>
#include <service/network/router.hpp>

#include <memory>

namespace soci
{
    class session;
} // namespace soci

/// @brief Главная служба
class Service
{
public:
    /// @brief Конструктор
    Service();

    /// @brief Запуск службы
    void Run();

private:
    /// @brief База данных
    std::shared_ptr<soci::session> sql_;

    /// @brief Таблица пользователей
    std::shared_ptr<UsersTable> usersTable_;

    /// @brief Маршрутизатор
    Router router_;
};
