include_directories(../../src/)

set(WS_TESTS_NAME Websocket_tests)
add_executable(${WS_TESTS_NAME} ./src/Websocket_tests.cpp ../../src/websocket/Websocket.cpp)
target_link_libraries(${WS_TESTS_NAME} GTest::gtest_main)
target_link_libraries(${WS_TESTS_NAME} ${Boost_LIBRARIES})
target_link_libraries(${WS_TESTS_NAME} ${OPENSSL_LIBRARIES})
add_test(NAME ${WS_TESTS_NAME} COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_websocket_tests_with_server.sh ${CMAKE_SOURCE_DIR})
