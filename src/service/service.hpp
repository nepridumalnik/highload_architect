#pragma once

#include <helpers/macro.hpp>

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
    EXPORT Service();

    /// @brief Запуск службы
    EXPORT void Run();

private:
    /// @brief База данных
    std::shared_ptr<soci::session> sql_;

    /// @brief Маршрутизатор
    Router router_;
};
