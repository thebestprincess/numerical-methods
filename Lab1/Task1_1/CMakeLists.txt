add_executable(Lab1_Task1_1
    main.cpp
    ${CMAKE_SOURCE_DIR}/Lab1/shared/IOUtils.cpp
    ${CMAKE_SOURCE_DIR}/Lab1/shared/LUSolver.cpp
    ${CMAKE_SOURCE_DIR}/Lab1/shared/Matrix.cpp
)

target_include_directories(Lab1_Task1_1 PRIVATE ${CMAKE_SOURCE_DIR}/Lab1/shared)