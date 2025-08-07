cmake_minimum_required(VERSION 3.31)
project(stratego)

set(CMAKE_CXX_STANDARD 23)

configure_file(
        ${CMAKE_SOURCE_DIR}/fonts/arial.ttf
        ${CMAKE_BINARY_DIR}/fonts/arial.ttf
        COPYONLY
)

add_executable(stratego main.cpp
        src/GameController/GameController.cpp
        src/GameController/GameController.h
        src/GameManager/GameManager.cpp
        src/GameManager/GameManager.h
        src/game/Board.cpp
        src/game/Board.h
        src/pieces/piece.h
        src/pieces/spy.h
        src/pieces/scout.h
        src/pieces/normalPiece.h
        src/pieces/miner.h
        src/pieces/flag.h
        src/pieces/bomb.h
        src/game/Player.h
        src/game/Player.cpp
        src/TextureManager/TextureManager.cpp
        src/TextureManager/TextureManager.h)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(stratego
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(stratego PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)

target_compile_definitions(stratego PRIVATE
        TEST_FILES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/fonts/"
)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(stratego PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)