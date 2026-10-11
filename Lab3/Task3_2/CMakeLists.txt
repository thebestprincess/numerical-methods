add_executable(Lab3_Task3_2
    src/main.cpp
    src/cubic_spline.cpp
    src/tridiagonal_solver.cpp
)

target_compile_features(Lab3_Task3_2 PRIVATE cxx_std_23)

target_include_directories(Lab3_Task3_2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Lab3_Task3_2 PRIVATE Matplot++::matplot)