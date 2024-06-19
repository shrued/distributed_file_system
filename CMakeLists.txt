cmake_minimum_required(VERSION 3.10)
project(DistributedFileSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost 1.70 REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

set(SERVER_SOURCES
    src/server.cpp
    src/file_system.cpp
    src/network.cpp
)

set(CLIENT_SOURCES
    src/client.cpp
    src/file_system.cpp
    src/network.cpp
)

set(HEADERS
    include/file_system.hpp
    include/network.hpp
)

add_executable(server ${SERVER_SOURCES} ${HEADERS})
add_executable(client ${CLIENT_SOURCES} ${HEADERS})

target_link_libraries(server Boost::system Boost::filesystem)
target_link_libraries(client Boost::system Boost::filesystem)

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
