#pragma once

#include <memory>
#include <functional>

#include "ExchangeTick.h"
#include "StorageManager.h"
#include "websocket/Websocket.h"

namespace qs {

    class BinanceQuoteService {
        public: 
            BinanceQuoteService(std::string host, 
                                std::string port,
                                std::string endpoint,
                                std::string order_book_api_url) :
                _host{host},
                _port{port},
                _endpoint{endpoint},
                _order_book_api_url{order_book_api_url}
            {}
            void begin_processing();
        private:
            const std::string _host;
            const std::string _port;
            const std::string _endpoint;
            const std::string _order_book_api_url;
            std::shared_ptr<ws::WebSocket> _ws;
            ws::WebSocket::on_connected_callback_t _on_connected;
            ws::WebSocket::on_read_callback_t _on_read;
            sm::StorageManager _storage_manager;

            void initalize_order_book();
            void add_tick(ExchangeTick tick);
    };

}
