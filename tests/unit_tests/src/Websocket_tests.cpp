#include "../../../src/websocket/Websocket.h"

#include <cstddef>
#include <gtest/gtest.h>


TEST(WebsocketTests, ConnectAndDisconectTest) {
    auto ws = std::make_shared<ws::WebSocket>("localhost", "8443", "/ws-api/v3", 
            [](std::shared_ptr<ws::WebSocket> ws) {
                const std::string message = "Hi there!";
                ws->async_write(message, [message](beast::error_code ec, std::size_t bytes_transferred) {
                        if (ec) {
                            std::cerr << "[!] Failed to write data : " << ec.message() << std::endl;
                            ASSERT_TRUE(false);
                        }
                        ASSERT_EQ(message.size(), bytes_transferred);
                });
            },
            [](std::shared_ptr<ws::WebSocket> ws, std::string&& message) {
                ASSERT_EQ("You sent: Hi there!", message);
                ws->async_close();
            }
    );
    ws->run();
    ASSERT_TRUE(true);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
