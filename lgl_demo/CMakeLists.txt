cmake_minimum_required(VERSION 4.2.3)

add_executable(Demo)

set_target_properties(Demo PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
)

set_target_properties(Demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    OUTPUT_NAME "Demo"
)

target_sources(Demo PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demo.cxx

    ${CMAKE_CURRENT_LIST_DIR}/SimulationScene.hxx
    ${CMAKE_CURRENT_LIST_DIR}/SimulationScene.cxx
    ${CMAKE_CURRENT_LIST_DIR}/SimulationInterface.hxx
    ${CMAKE_CURRENT_LIST_DIR}/SimulationInterface.cxx

    ${CMAKE_CURRENT_LIST_DIR}/SponzaScene.hxx
    ${CMAKE_CURRENT_LIST_DIR}/SponzaScene.cxx
    ${CMAKE_CURRENT_LIST_DIR}/LightingInterface.hxx
    ${CMAKE_CURRENT_LIST_DIR}/LightingInterface.cxx
)

target_link_libraries(Demo PRIVATE LglEng)
target_link_libraries(Demo PRIVATE LglDeps)