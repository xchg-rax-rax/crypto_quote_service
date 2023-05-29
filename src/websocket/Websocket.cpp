#include "Websocket.h"

#include <iostream>
#include <boost/beast/core/stream_traits.hpp>

namespace ws {

void WebSocket::run() {
    // need exceptions for error handling
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

    // Set the timeout for the operation
    beast::get_lowest_layer(_ws).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(_ws).async_connect(results,
        beast::bind_front_handler(&WebSocket::on_connect, shared_from_this()));
}

void WebSocket::on_connect(beast::error_code ec, asio::ip::tcp::resolver::results_type::endpoint_type ep) {
    if(ec) {
        std::cerr << "[!] Failed to connect to host : " << ec.message() << std::endl;
        return;
    }

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(_ws).expires_never();

    // Set suggested timeout settings for the websocket
    _ws.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    _ws.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::request_type& req)
        {
            req.set(beast::http::field::user_agent, 
                    std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async");
        }));

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    _host += ":" + std::to_string(ep.port());

    // Perform the SSL handshake
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

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(_ws).expires_never();

    // Set suggested timeout settings for the websocket
    _ws.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    _ws.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::request_type& req) {
            req.set(beast::http::field::user_agent,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-async-ssl");
        }
    ));

    // Perform the websocket handshake
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
    
    // start reading
    _ws.async_read(
        _buffer,
        beast::bind_front_handler(
            &WebSocket::on_read,
            shared_from_this()));
}

/*
void WebSocket::on_write( beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec) {
        std::cerr << "[!] Failed to write data : " << ec.message() << std::endl;
        return;
    }
    
    // Read a message into our buffer
    _ws.async_read(
        _buffer,
        beast::bind_front_handler(
            &WebSocket::on_read,
            shared_from_this()));
}
*/

void WebSocket::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec) {
        std::cerr << "[!] Failed to read data : " << ec.message() << std::endl;
        return;
    }

    std::cout << beast::make_printable(_buffer.data()) << std::endl;

    // Close the WebSocket connection
    _ws.async_read(
        _buffer,
        beast::bind_front_handler(
            &WebSocket::on_read,
            shared_from_this()));
}

void WebSocket::on_close(beast::error_code ec) {
    if(ec) {
        std::cerr << "[!] Failed to close data : " << ec.message() << std::endl;
        return;
    }

    // If we get here then the connection is closed gracefully

    // The make_printable() function helps print a ConstBufferSequence
    std::cout << beast::make_printable(_buffer.data()) << std::endl;

    std::cout << "[-] Closed Connection" << std::endl;
}

}

