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

    auto it = controllers_.find(req.target());

    if (it != controllers_.end())
    {
        http::response<http::string_body> res;

        it->second->HandleRequest(req, res);

        res.prepare_payload();
        http::write(socket, res);
    }
}

void Router::AddController(const std::string &route, std::unique_ptr<AbstractController> controller)
{
    controllers_.insert({route, std::move(controller)});
}
