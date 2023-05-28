#pragma once

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <thread>
#include <string_view>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/error.hpp>

namespace ws {

class WebSocket {
    public:
        WebSocket(const std::string_view host, 
                  const std::string_view port, 
                  const std::string_view target,
                  boost::asio::io_context& ioc) : 
            _host{host},
            _port{port},
            _target{target},
            _ws{ioc},
            _resovler{ioc}
        {
        }

        bool connect();
        bool read(std::string& message);
        bool write(const std::string_view message);
        void close();
    private:
        const std::string_view _host;
        const std::string_view _port;
        const std::string_view _target;
        boost::beast::websocket::stream<boost::asio::ip::tcp::socket> _ws;
        boost::asio::ip::tcp::resolver _resovler;
};
}
