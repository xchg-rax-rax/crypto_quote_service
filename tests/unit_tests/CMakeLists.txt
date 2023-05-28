include_directories(../../src/)

set(WS_TESTS_NAME Websocket_tests)
add_executable(${WS_TESTS_NAME} ./src/Websocket_tests.cpp ../../src/websocket/Websocket.cpp)
target_link_libraries(${WS_TESTS_NAME} GTest::gtest_main)
add_test(NAME ${WS_TESTS_NAME} COMMAND Websocket_tests ${CMAKE_CURRENT_SOURCE_DIR}/scripts/ws_server.py)
