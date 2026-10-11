add_executable(Lab2_Task2_2
    src/main.cpp
)

target_compile_features(Lab2_Task2_2 PRIVATE cxx_std_23)

target_include_directories(Lab2_Task2_2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)