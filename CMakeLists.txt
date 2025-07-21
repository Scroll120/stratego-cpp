cmake_minimum_required(VERSION 3.28)

project(stratego)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        include/Game/
        include/UI/
)

add_subdirectory(source)