#pragma once

#include <vector>
#include <string>

/// @brief Просчитать MD5 Хэш
/// @param input Данные
/// @return Строка с хэшем
std::string HashMD5(const std::vector<uint8_t> &input);

/// @brief Просчитать MD5 Хэш
/// @param input Строка
/// @return Строка с хэшем
std::string HashMD5(const std::string &input);

enum HttpMethod
{
    Unknown,
    Post,
    Get,
    Delete
};

/// @brief Получить метод в виде enum
/// @param method Строка с методом
/// @return Enum значение
HttpMethod GetMethod(const std::string &method);
