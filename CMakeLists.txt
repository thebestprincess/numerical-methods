cmake_minimum_required(VERSION 3.20)

project(NumericalMethods)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
    GIT_TAG        v1.2.2
)
FetchContent_MakeAvailable(matplotplusplus)

if(TARGET matplot)
    target_compile_options(matplot PRIVATE -Wno-error=unused-but-set-variable -Wno-error)
endif()

add_subdirectory(Lab1)
add_subdirectory(Lab2)
add_subdirectory(Lab3)