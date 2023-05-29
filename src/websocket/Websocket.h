#pragma once

#include <atomic>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <functional>
#include <memory>
#include <iostream>

using namespace boost;

namespace ws {


class WebSocket : public std::enable_shared_from_this<WebSocket>{
    typedef std::function<void(beast::error_code ec, std::size_t bytes_transferred)> on_write_callback_t;
    typedef std::function<void(std::shared_ptr<WebSocket>)> on_connected_callback_t;
    typedef std::function<void(std::shared_ptr<WebSocket>, std::string&&)> on_read_callback_t;
    public:
        WebSocket(std::string host,
                  const std::string_view port,
                  const std::string_view endpoint,
                  asio::io_context& ioc, 
                  asio::ssl::context& ctx,
                  on_connected_callback_t on_connected_callback,
                  on_read_callback_t on_read_callback
                  ) : 
            _host{std::move(host)},
            _port{port},
            _endpoint{endpoint},
            _ioc{ioc},
            _resolver{asio::make_strand(_ioc)},
           _ws{asio::make_strand(_ioc), ctx},
           _on_connected_callback{on_connected_callback},
           _on_read_callback{on_read_callback}
        {}
        void run();
        const std::atomic<bool>& is_connected() const { return _connected; }
        void async_write(std::string message, on_write_callback_t on_write_callback);
        ~WebSocket() {
            // Should probably implement graceful closing here
            std::cout << "Websocket destroyed" << std::endl;
        }
    private:
        std::atomic<bool> _connected{false};
        std::string _host;
        const std::string_view _port;
        const std::string_view _endpoint;
        asio::io_context& _ioc;
        asio::ip::tcp::resolver _resolver;
        beast::websocket::stream<asio::ssl::stream<beast::tcp_stream>,true> _ws;
        // Note that if these do a considerable amount of work or lock etc. they will block the ioc
        on_connected_callback_t _on_connected_callback;
        on_read_callback_t _on_read_callback;
        beast::flat_buffer _buffer;


        void on_resolve(beast::error_code ec, asio::ip::tcp::resolver::results_type results);
        void on_connect(beast::error_code ec, asio::ip::tcp::resolver::results_type::endpoint_type ep);
        void on_ssl_handshake(beast::error_code ec);
        void on_handshake(beast::error_code ec);
        void on_write(beast::error_code ec, std::size_t bytes_transferred);
        void on_read(beast::error_code ec, std::size_t bytes_transferred);
        void on_close(beast::error_code ec);

        static std::string flat_buffer_to_string(const beast::flat_buffer& bufffer);
};
}
