cmake_minimum_required(VERSION 3.20)

project(NumericalMethods)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(MATPLOTPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MATPLOTPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
    GIT_TAG        v1.2.0
)
FetchContent_MakeAvailable(matplotplusplus)

add_subdirectory(Lab1)
add_subdirectory(Lab2)
add_subdirectory(Lab3)