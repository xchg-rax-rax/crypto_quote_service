cmake_minimum_required(VERSION 3.23.3)
cmake_policy(SET CMP0135 NEW)

# Configure Complier
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)

# Configure Project
project(crypto_query_service)
add_executable(${PROJECT_NAME} src/main.cpp src/websocket/Websocket.cpp)

# Install Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.81 REQUIRED COMPONENTS program_options)
message(STATUS "Boost version: ${Boost_VERSION}")
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Configure GTest and unit tests 
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()
add_subdirectory(./tests/unit_tests)
