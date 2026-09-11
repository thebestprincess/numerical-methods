add_executable(Lab1_Task1_2
    main.cpp
    IOUtils.cpp
    TridiagonalSolver.cpp
)

target_include_directories(Lab1_Task1_2 PRIVATE ${CMAKE_SOURCE_DIR}/shared)