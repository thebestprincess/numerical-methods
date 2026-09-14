add_executable(Lab1_Task1_4
    main.cpp
    JacobiSolver.cpp
    ${CMAKE_SOURCE_DIR}/shared/Matrix.cpp
    ${CMAKE_SOURCE_DIR}/shared/IOUtils.cpp
    ${CMAKE_SOURCE_DIR}/shared/LUSolver.cpp
)


target_include_directories(Lab1_Task1_4 PRIVATE ${CMAKE_SOURCE_DIR}/shared)