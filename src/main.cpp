#include <memory>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>

#include <boost/beast/core/error.hpp>


#include "websocket/Websocket.h"
#include "BinanceQuoteService.h"

int main() {
    auto binance_qs = qs::BinanceQuoteService("stream.binance.com", 
                                              "9443","/ws/bnbbtc@depth", 
                                              "https://api.binance.com/api/v3/depth?symbol=BNBBTC&limit=1000");
    binance_qs.begin_processing();
    return EXIT_SUCCESS;
}
