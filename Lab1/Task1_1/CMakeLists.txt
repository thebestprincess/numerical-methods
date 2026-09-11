add_executable(Lab1_Task1_1
    main.cpp
    IOUtils.cpp
    LUSolver.cpp
    Matrix.cpp
)

target_include_directories(Lab1_Task1_1 PRIVATE ${CMAKE_SOURCE_DIR}/shared)