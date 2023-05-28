#include "websocket/Websocket.h"
#include <boost/asio/io_context.hpp>
#include <boost/beast/core/error.hpp>
#include <cstddef>
#include <cstdlib>

int main() {
    boost::asio::io_context ioc;
    auto ws = ws::WebSocket("127.0.0.1", "8888", "/", ioc);
    auto handler_thread = std::thread([&](){
                ioc.run();
            });
    if (!ws.connect()) {
        return EXIT_FAILURE;
    }
    if (!ws.write("Hello world")) {
        return EXIT_FAILURE;
    }
    std::string message;
    if(!ws.read(message)) {
        return EXIT_FAILURE;
    }
    std::cout << "[+] Received Response -> "  << message << std::endl;
    ws.close();
    handler_thread.join();
    return EXIT_SUCCESS;
}
