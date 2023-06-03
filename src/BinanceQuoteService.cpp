#include "BinanceQuoteService.h"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <cpr/cpr.h>

namespace qs {

    void BinanceQuoteService::initalize_order_book() {
        auto response = cpr::Get(cpr::Url{_order_book_api_url});
        std::cout << response.text << std::endl;
    }
    // do we want this function to be blocking or non blocking?
    void BinanceQuoteService::begin_processing() {
        initalize_order_book();
        _on_connected = [this](std::shared_ptr<ws::WebSocket> ws) {
            std::cout << "Connected" << std::endl;
        };
        _on_read = [this](std::shared_ptr<ws::WebSocket> ws, std::string&& message) {
            auto tick = ExchangeTick(message);
            this->add_tick(std::move(tick));
        };
        _ws = std::make_shared<ws::WebSocket>(_host, _port, _endpoint, _on_connected, _on_read);
        _ws->run();
    }


    void BinanceQuoteService::add_tick(ExchangeTick tick) {
        std::cout << "Tick" << std::endl;
        _storage_manager.add_tick(std::move(tick));
    }

}
