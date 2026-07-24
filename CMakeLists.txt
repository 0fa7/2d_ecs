cmake_minimum_required(VERSION 3.10)

project(2d_ecs VERSION 0.1)

find_package(Lua REQUIRED)

include_directories(libs/
    libs/glm
    src/
    ${LUA_INCLUDE_DIR})

set(2D_ECS_SOURCES
    src/Components/TransformComponent.hpp
    src/ECS/ECS.cpp
    src/ECS/ECS.hpp
    src/Logger/Logger.cpp
    src/Logger/Logger.hpp
    src/Game/Game.cpp
    src/Game/Game.hpp
    src/Main.cpp
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(2d_ecs
    ${2D_ECS_SOURCES})

#target_compile_options(2d_ecs PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(2d_ecs
    lua5.3
    SDL2
    SDL2_image
    SDL2_ttf)

