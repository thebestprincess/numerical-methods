add_executable(Lab1_Task1_3
    main.cpp
    IterativeSolver.cpp
    ${CMAKE_SOURCE_DIR}/shared/IOUtils.cpp
    ${CMAKE_SOURCE_DIR}/shared/LUSolver.cpp
    ${CMAKE_SOURCE_DIR}/shared/Matrix.cpp
)

target_include_directories(Lab1_Task1_3 PRIVATE ${CMAKE_SOURCE_DIR}/shared)