#pragma once

#include <helpers/macro.hpp>

#include <service/network/router.hpp>

#include <soci/connection-pool.h>

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
    EXPORT Service();

    /// @brief Запуск службы
    EXPORT void Run();

private:
    /// @brief Пул соединений
    std::shared_ptr<soci::connection_pool> pool_;

    /// @brief Маршрутизатор
    Router router_;
};
