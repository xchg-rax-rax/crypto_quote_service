#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <memory>

using namespace boost;

namespace ws {
class WebSocket : public std::enable_shared_from_this<WebSocket>{
    public:
        WebSocket(std::string host,
                  const std::string_view port,
                  const std::string_view endpoint,
                  asio::io_context& ioc, 
                  asio::ssl::context& ctx) : 
            _host{std::move(host)},
            _port{port},
            _endpoint{endpoint},
            _ioc{ioc},
            _resolver{asio::make_strand(_ioc)},
           _ws{asio::make_strand(_ioc), ctx} 
        {}
        void run();
    private:
        std::string _host;
        const std::string_view _port;
        const std::string_view _endpoint;
        asio::io_context& _ioc;
        asio::ip::tcp::resolver _resolver;
        beast::websocket::stream<asio::ssl::stream<beast::tcp_stream>,true> _ws;
        beast::flat_buffer _buffer;

        void on_resolve(beast::error_code ec, asio::ip::tcp::resolver::results_type results);
        void on_connect(beast::error_code ec, asio::ip::tcp::resolver::results_type::endpoint_type ep);
        void on_ssl_handshake(beast::error_code ec);
        void on_handshake(beast::error_code ec);
        void on_write( beast::error_code ec, std::size_t bytes_transferred);
        void on_read(beast::error_code ec, std::size_t bytes_transferred);
        void on_close(beast::error_code ec);
};
}
