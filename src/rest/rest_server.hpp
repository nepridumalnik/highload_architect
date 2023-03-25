#pragma once

#include <models/users/users.hpp>

#include <helpers/macro.hpp>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/ServerApplication.h>

#include <memory>

class RestServer : public Poco::Util::ServerApplication
{
public:
    /// @brief Конструктор
    /// @param pool
    EXPORT explicit RestServer(std::shared_ptr<Poco::Data::SessionPool> pool = nullptr);

    /// @brief Запуск
    void EXPORT Start();

    /// @brief Ожидание запроса на выключение
    void EXPORT WaitForTerminationRequest();

private:
    /// @brief HTTP сервер
    Poco::SharedPtr<Poco::Net::HTTPServer> server_;

    /// @brief Обработчик запросов
    Poco::Net::HTTPRequestHandlerFactory::Ptr requestHandler_;

    /// @brief Пул соединений
    std::shared_ptr<Poco::Data::SessionPool> pool_;
};
