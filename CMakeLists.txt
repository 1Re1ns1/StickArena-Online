cmake_minimum_required(VERSION 3.21)
project(StickArena LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(StickArena apps/Client/main.cpp
        src/core/Application.cpp
        include/stickarena/core/Application.h
        src/ecs/system/MovementSystem.cpp
        include/stickarena/ecs/system/MovementSystem.h
        src/physics/physicsHero.cpp
        include/stickarena/physics/physicsHero.h
        src/ecs/system/Collision.cpp
        include/stickarena/ecs/system/Collision.h)

target_include_directories(StickArena PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System Audio Network)
target_link_libraries(StickArena PRIVATE
        SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

find_package(entt CONFIG REQUIRED)
target_link_libraries(StickArena PRIVATE EnTT::EnTT)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copying assets to build dir"
)

# после add_executable(...)
add_dependencies(StickArena copy_assets)   # StickArena — имя твоего exe target
if(WIN32)
    add_custom_command(TARGET StickArena POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "C:/dev/vcpkg/installed/x64-mingw-dynamic/bin"
            "$<TARGET_FILE_DIR:StickArena>")
endif()
