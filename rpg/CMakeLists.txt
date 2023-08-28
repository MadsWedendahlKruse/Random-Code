cmake_minimum_required(VERSION 3.21)
project(rpg LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(json 
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(include)
include_directories(lib)
include_directories(build)

set(SOURCES
    src/main.cpp
    # Engine
    src/engine/tile.cpp
    src/engine/aabb.cpp
    src/engine/game_object.cpp
    src/engine/mobile_object.cpp
    src/engine/entity.cpp
    src/engine/player.cpp
    src/engine/world.cpp
    src/engine/input_handler.cpp
    # Game State
    src/engine/game_state/game_state_manager.cpp
    src/engine/game_state/states/game_state.cpp
    src/engine/game_state/states/startup_state.cpp
    src/engine/game_state/states/loading_state.cpp
    src/engine/game_state/states/world_state.cpp
    # UI
    src/engine/ui/ui_manager.cpp
    src/engine/ui/elements/ui_element.cpp
    src/engine/ui/elements/button.cpp
    src/engine/ui/elements/button_list.cpp
    # Resources
    src/resources/animation.cpp
    src/resources/weighted_texture.cpp
    src/resources/connected_textures/connected_texture.cpp
    src/resources/connected_textures/blob_texture.cpp
    src/resources/connected_textures/fence_texture.cpp
    src/resources/vertex_quad.cpp
    src/resources/resource_manager.cpp
    src/resources/game_registry.cpp
)

add_executable(rpg ${SOURCES})

target_link_libraries(rpg PRIVATE
    sfml-graphics 
    nlohmann_json::nlohmann_json
)
target_compile_features(rpg PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET rpg POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:rpg> $<TARGET_FILE_DIR:rpg> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS rpg)
