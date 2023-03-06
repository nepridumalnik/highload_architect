#include <service/network/router.hpp>

#include <boost/beast.hpp>

#include <iostream>

void Router::operator()(boost::asio::ip::tcp::socket &socket)
{
    using namespace boost::beast;

    flat_buffer buf;
    http::request<http::dynamic_body> req;

    try
    {
        http::read(socket, buf, req);
    }
    catch (const boost::beast::http::error &e)
    {
        std::cerr << "Error reading HTTP request: " << static_cast<int>(e) << std::endl;
        return;
    }

    const std::string target = req.target();

    for (auto &controller : controllers_)
    {
        if (controller->HasRoute(target))
        {
            http::response<http::string_body> res;

            controller->HandleRequest(req, res);

            res.prepare_payload();
            http::write(socket, res);

            break;
        }
    }
}

void Router::AddController(std::unique_ptr<AbstractController> controller)
{
    controllers_.push_back(std::move(controller));
}
