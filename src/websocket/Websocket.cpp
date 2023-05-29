#include "Websocket.h"

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/websocket/rfc6455.hpp>
#include <iostream>
#include <boost/beast/core/stream_traits.hpp>

namespace ws {

void WebSocket::run() {
    _resolver.async_resolve(
        _host,
        _port,
        beast::bind_front_handler(
            &WebSocket::on_resolve,
            shared_from_this()));
}


void WebSocket::on_resolve(beast::error_code ec, asio::ip::tcp::resolver::results_type results) {
    if(ec) {
        std::cerr << "[!] Failed to resolve host : " << ec.message() << std::endl;
        return;
    }

    beast::get_lowest_layer(_ws).expires_after(std::chrono::seconds(30));

    beast::get_lowest_layer(_ws).async_connect(results,
        beast::bind_front_handler(&WebSocket::on_connect, shared_from_this()));
}

void WebSocket::on_connect(beast::error_code ec, asio::ip::tcp::resolver::results_type::endpoint_type ep) {
    if(ec) {
        std::cerr << "[!] Failed to connect to host : " << ec.message() << std::endl;
        return;
    }

    beast::get_lowest_layer(_ws).expires_never();
    _ws.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::client));

    _ws.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::request_type& req)
        {
            req.set(beast::http::field::user_agent, 
                    std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async");
        }));

    _host += ":" + std::to_string(ep.port());

    _ws.next_layer().async_handshake(
        asio::ssl::stream_base::client,
        beast::bind_front_handler(
            &WebSocket::on_ssl_handshake,
            shared_from_this()));
}

void WebSocket::on_ssl_handshake(beast::error_code ec) {
    if(ec) {
        std::cerr << "[!] Failed to perform SSL handshake : " << ec.message() << std::endl;
        return;
    }

    beast::get_lowest_layer(_ws).expires_never();

    _ws.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::client));

    _ws.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::request_type& req) {
            req.set(beast::http::field::user_agent,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-async-ssl");
        }
    ));

    _ws.async_handshake(_host, _endpoint,
        beast::bind_front_handler(
            &WebSocket::on_handshake,
            shared_from_this()));
}



void WebSocket::on_handshake(beast::error_code ec) {
    if(ec) {
        std::cerr << "[!] Failed to perform handshake : " << ec.message() << std::endl;
        return;
    }

    std::cout << "[+] Connection established with host : " << _host << std::endl;
    _connected = true;
    // start reading
    _ws.async_read(
        _buffer,
        beast::bind_front_handler(
            &WebSocket::on_read,
            shared_from_this()));
    // Call the on connected callback
    // Find a way to do this async
    _on_connected_callback(shared_from_this());
}

void WebSocket::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec) {
        std::cerr << "[!] Failed to read data : " << ec.message() << std::endl;
        return;
    }

    std::cout << beast::make_printable(_buffer.data()) << std::endl;

    _ws.async_read(
        _buffer,
        beast::bind_front_handler(
            &WebSocket::on_read,
            shared_from_this()));
    // Call the on connected callback
    // Find a way to do this async
    _on_read_callback(shared_from_this(), flat_buffer_to_string(_buffer));
}

void WebSocket::on_close(beast::error_code ec) {
    if(ec) {
        std::cerr << "[!] Failed to close data : " << ec.message() << std::endl;
        return;
    }

    std::cout << "[-] Closed Connection" << std::endl;
}


void WebSocket::async_close() {
    if (!_connected) {
        std::cerr << "[!] async_close failed : Websocket connection has not yet been established" << std::endl;
        return;
    }
    _ws.async_close(
        beast::websocket::close_code::normal,
        beast::bind_front_handler(
            &WebSocket::on_close,
            shared_from_this()));
}


void WebSocket::async_write(std::string message, on_write_callback_t on_write_callback) {
    if (!_connected) {
        std::cerr << "[!] async_write failed : Websocket connection has not yet been established" << std::endl;
        return;
    }
    _ws.async_write(asio::buffer(message), on_write_callback);
}

std::string WebSocket::flat_buffer_to_string(const beast::flat_buffer& buffer) {
    boost::asio::streambuf sb;
    sb.sputn(boost::beast::buffers_to_string(buffer.data()).c_str(), buffer.size());
    std::istream is(&sb);
    std::string s((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    return s;
}
}

