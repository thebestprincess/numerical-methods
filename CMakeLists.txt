cmake_minimum_required(VERSION 3.20)

project(NumericalMethods LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(MATPLOTPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MATPLOTPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
    GIT_TAG        master
)
FetchContent_MakeAvailable(matplotplusplus)

if(TARGET matplot)
    target_compile_options(matplot PRIVATE -Wno-error)
endif()


add_subdirectory(Lab1)
add_subdirectory(Lab2)
add_subdirectory(Lab3)