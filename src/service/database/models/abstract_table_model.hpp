#pragma once

#include <string>

/// @brief Интерфейс модели
/// @tparam T тип структуры
/// @tparam F структура для поиска
/// @tparam Db база данных
template <typename T, typename F, typename Db>
struct AbstractTableModel
{
    /// @brief Виртуальный деструктор
    virtual ~AbstractTableModel() = default;

    /// @brief Вставить строку
    /// @param Строка
    /// @return True, если удачно
    virtual bool Insert(const T &row) = 0;

    /// @brief Найти строку по идентификатору
    /// @param id Идентификатор
    /// @param row Структура строки
    /// @return True, если удачно
    virtual bool FindById(const size_t id, T &row) = 0;

    /// @brief Найти строку по структуре для поиска
    /// @param f Структура для поиска
    /// @param row Строка
    /// @return True, если удачно
    /// @details Поиск рекомендуется производить по уникальным полям
    virtual bool FindByCondition(const F &f, T &row) = 0;

    /// @brief Удалить пользователя по идентификатору
    /// @param id Идентификатор
    /// @return True, если удачно
    virtual bool Delete(const size_t id) = 0;

    /// @brief Получить родительскую базу данных
    /// @return База данных
    virtual Db GetDatabase() = 0;
};
