add_executable(Lab1_Task1_5
    main.cpp
    QRSolver.cpp
    ${CMAKE_SOURCE_DIR}/Lab1/shared/Matrix.cpp
    ${CMAKE_SOURCE_DIR}/Lab1/shared/IOUtils.cpp
    ${CMAKE_SOURCE_DIR}/Lab1/shared/LUSolver.cpp
)

target_include_directories(Lab1_Task1_5 PRIVATE ${CMAKE_SOURCE_DIR}/Lab1/shared)