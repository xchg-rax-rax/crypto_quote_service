#include "Websocket.h"

#include <boost/beast/core/bind_handler.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/role.hpp>
#include <boost/beast/core/stream_traits.hpp>
#include <boost/beast/websocket/rfc6455.hpp>
#include <boost/beast/websocket/stream_base.hpp>
#include <chrono>
#include <iostream>
#include <exception>
#include <system_error>

//#include <boost/beast/websocket/rfc6455.hpp>

namespace ws {

bool WebSocket::connect() {
    try {
        const auto results = _resovler.resolve(_host, _port);
        boost::asio::connect(_ws.next_layer(), results.begin(), results.end());
        _ws.handshake(_host, _target);
    }
    catch (const std::exception& e) {
        std::cerr << "[!] Failed to connect to host " << _host << ":" << _port << " : " << e.what() << std::endl;
        return false;
    }
    std::cout << "[+] Connected to websocket" << std::endl;
    return true;
}


bool WebSocket::read(std::string& message) {
    try {
        boost::beast::flat_buffer buffer;
        boost::beast::error_code ec;
        _ws.read(buffer, ec);
        if (ec) {
            std::cerr << "[!] Read failed : " << ec.message() << std::endl;
            return false;
        }
        message = boost::beast::buffers_to_string(buffer.data());
    }
    catch (const std::system_error& e) {
        std::cerr << "[!] Read failed : " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool WebSocket::write(const std::string_view message) {
    try {
        boost::beast::error_code ec;
        _ws.write(boost::asio::buffer(message), ec);
        if (ec) {
            std::cerr << "[!] Write failed : " << ec.message() << std::endl;
            return false;
        }
    }
    catch (const std::system_error& e) {
        std::cerr << "[!] Write failed : " << e.what() << std::endl;
        return false;
    }
    return true;
}


void WebSocket::close() {
    _ws.close(boost::beast::websocket::close_code::normal);
    std::cout << "[-] Closed websocket connection to " << _host << ":" << _port << std::endl;
}

}
