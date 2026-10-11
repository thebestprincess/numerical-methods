add_executable(Lab3_Task3_1
    src/main.cpp
)

target_compile_features(Lab3_Task3_1 PRIVATE cxx_std_23)

target_include_directories(Lab3_Task3_1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Lab3_Task3_1 PRIVATE Matplot++::matplot)