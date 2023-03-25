#pragma once

#include <nlohmann/json.hpp>

#include <string>

/// @brief Структура, преобразуемая в JSON и обратно
struct AbstractTableRow
{
    /// @brief Деструктор
    virtual ~AbstractTableRow() = default;

    /// @brief Получить из JSON объекта
    /// @param json JSON объект
    /// @return True, если удачно
    virtual bool FromJson(const std::string &json) = 0;

    /// @brief Конвертировать в JSON объект
    /// @return JSON объект
    virtual nlohmann::json ToJson() const = 0;

    /// @brief Конвертировать в JSON объект
    /// @return Строка с JSON объектом
    virtual std::string ToString() const = 0;

    /// @brief Валидация структуры
    /// @return True, если всё хорошо
    virtual bool Validate() const = 0;
};
