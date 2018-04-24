cmake_minimum_required(VERSION 3.4)

project(openvpn-ice-client)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-w)

add_library(openvpn-ice-client SHARED ${PROJECT_SOURCE_DIR}/src/main.c)

if (WIN32)
    add_dependencies(openvpn-ice-client kernel32 wsock32 ws2_32)
    target_link_libraries(openvpn-ice-client kernel32 wsock32 ws2_32)
endif()

if (UNIX AND NOT ANDROID_PLATFORM)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(openvpn-ice-client Threads::Threads)
endif()
