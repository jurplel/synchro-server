cmake_minimum_required(VERSION 3.12)

project(synchro-server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(QT_DEPRECATED_WARNINGS)

set(SOURCES
    main.cpp
    server.cpp
)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Network)