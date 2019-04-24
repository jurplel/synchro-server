cmake_minimum_required(VERSION 3.12)

project(synchro-server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(ASIO_STANDALONE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES
    main.cpp
    server.cpp
)

find_path(ASIO_ROOT asio.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_include_directories(${PROJECT_NAME} PRIVATE ${ASIO_ROOT})