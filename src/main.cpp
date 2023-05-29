#include "websocket/Websocket.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/beast/core/error.hpp>
#include <cstddef>
#include <cstdlib>
#include <iostream>

int main() {
    boost::asio::io_context ioc;
    asio::ssl::context ctx(asio::ssl::context::sslv23);
    // may need to do some additional set up of the ssl context here in order to establish a connection
    // wss://stream.binance.com:9443/ws/btcusdt@trade
    auto ws = std::make_shared<ws::WebSocket>("stream.binance.com", "9443", "/ws/btcusdt@trade", ioc, ctx);
    //auto ws = std::make_shared<ws::WebSocket>("ws-api.binance.com", "443", "/ws-api/v3", ioc, ctx);
    //auto ws = std::make_shared<ws::WebSocket>("localhost", "8443", "/ws-api/v3", ioc, ctx);
    auto handler_thread = std::thread([&](){
                ioc.run();
            });
    ws->run();

    handler_thread.join();
    return EXIT_SUCCESS;
}
