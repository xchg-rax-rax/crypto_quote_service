#include "websocket/Websocket.h"

#include <memory>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>

#include <boost/beast/core/error.hpp>

int main() {
    // wss://stream.binance.com:9443/ws/btcusdt@trade
    //auto ws = std::make_shared<ws::WebSocket>("stream.binance.com", "9443", "/ws/btcusdt@trade", ioc, ctx);
    //auto ws = std::make_shared<ws::WebSocket>("ws-api.binance.com", "443", "/ws-api/v3", ioc, ctx);
    auto ws = std::make_shared<ws::WebSocket>("localhost", "8443", "/ws-api/v3", 
            [](std::shared_ptr<ws::WebSocket> ws) {
                ws->async_write("Hi there!", [](beast::error_code ec, std::size_t bytes_transferred) {
                        if (ec) {
                            std::cerr << "[!] Failed to write data : " << ec.message() << std::endl;
                        }
                        std::cout << "Wrote " << bytes_transferred << " bytes" << std::endl;
                });
            },
            [](std::shared_ptr<ws::WebSocket> ws, std::string&& message) {
                std::cout << "[+] Got message : " << message << std::endl;
                ws->async_close();
            }
    );

    ws->run();
    return EXIT_SUCCESS;
}
