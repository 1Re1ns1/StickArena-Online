cmake_minimum_required(VERSION 3.21)
project(StickArena LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(StickArena apps/Client/main.cpp
        src/core/Application.cpp
        include/stickarena/core/Application.h
        src/ecs/system/MovementSystem.cpp
        include/stickarena/ecs/system/MovementSystem.h)

target_include_directories(StickArena PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System Audio Network)
target_link_libraries(StickArena PRIVATE
        SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

if(WIN32)
    add_custom_command(TARGET StickArena POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "C:/dev/vcpkg/installed/x64-mingw-dynamic/bin"
            "$<TARGET_FILE_DIR:StickArena>")
endif()
