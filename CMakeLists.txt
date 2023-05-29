cmake_minimum_required(VERSION 3.23.3)
cmake_policy(SET CMP0135 NEW)

# Configure Complier
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)

# Configure Project
project(crypto_query_service)
add_executable(${PROJECT_NAME} src/main.cpp src/websocket/Websocket.cpp)

# Install OpenSSL

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIR})
  message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
  message(FATAL_ERROR "OpenSSL not found")
endif()
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

# Install Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
# The following line could probably be modified to reduce binary size, depends on how the linker behaves
find_package(Boost 1.81 REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Configure GTest and unit tests 
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()
add_subdirectory(./tests/unit_tests)
