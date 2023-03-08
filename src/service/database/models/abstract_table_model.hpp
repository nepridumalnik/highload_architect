#pragma once

#include <string>

/// @brief Структура, преобразуемая в JSON и обратно
struct AbstractJsonStruct
{
    /// @brief Получить из JSON объекта
    /// @param json JSON объект
    /// @return True, если удачно
    virtual bool FromJson(const std::string &json) = 0;

    /// @brief Конвертировать в JSON объект
    /// @return JSON объект
    virtual std::string ToJson() = 0;
};

/// @brief Интерфейс модели
/// @tparam T тип структуры
template <typename T>
struct AbstractTableModel
{
    /// @brief Виртуальный деструктор
    virtual ~AbstractTableModel() = default;

    /// @brief Вставить строку
    /// @param Строка
    virtual void Insert(const T &row) = 0;

    /// @brief Найти строку по идентификатору
    /// @param id Идентификатор
    /// @param Строка
    /// @return True, если удачно
    virtual bool FindById(const int id, T &row) = 0;

    /// @brief Удалить пользователя по идентификатору
    /// @param id Идентификатор
    virtual void Delete(const int id) = 0;
};
