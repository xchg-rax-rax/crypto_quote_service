#include "../../../src/websocket/Websocket.h"

#include <boost/beast/core/error.hpp>
#include <cstddef>
#include <gtest/gtest.h>


TEST(WebsocketTests, ConnectAndDisconectTest) {
    boost::asio::io_context ioc;
    auto ws = ws::WebSocket("127.0.0.1", "8888", "/", ioc);
    auto handler_thread = std::thread([&](){
                ioc.run();
            });
    ASSERT_TRUE(ws.connect());
    ASSERT_TRUE(ws.write("Hello world"));
    std::string message;
    ASSERT_TRUE(ws.read(message));
    ASSERT_EQ(message, "You sent: Hello World");
    ws.close();
    handler_thread.join();
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
