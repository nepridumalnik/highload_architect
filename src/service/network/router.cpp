#include <service/network/router.hpp>

#include <boost/beast.hpp>

#include <iostream>

namespace http = boost::beast::http;

void Router::operator()(boost::asio::ip::tcp::socket &socket)
{
    using namespace boost::beast;

    flat_buffer buf;
    http::request<http::dynamic_body> req;

    try
    {
        http::read(socket, buf, req);
    }
    catch (const http::error &e)
    {
        std::cerr << "Error reading HTTP request: " << static_cast<int>(e) << std::endl;
        return;
    }

    const std::string route = req.target();

    http::response<http::string_body> res;

    for (auto &controller : controllers_)
    {
        if (controller->HandleRequest(route, req, res))
        {
            res.prepare_payload();
            http::write(socket, res);
            return;
        }
    }

    res.result(http::status::not_found);
    res.prepare_payload();
    http::write(socket, res);
}

void Router::AddController(std::unique_ptr<AbstractController> controller)
{
    controllers_.push_back(std::move(controller));
}
