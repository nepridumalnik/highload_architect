#pragma once

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
