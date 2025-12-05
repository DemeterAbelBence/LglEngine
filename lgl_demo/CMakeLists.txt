add_executable(Demo)

set_target_properties(Demo PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_target_properties(Demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    OUTPUT_NAME "Demo"
)

target_sources(Demo PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demo.cpp

    ${CMAKE_CURRENT_LIST_DIR}/SimulationScene.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SimulationScene.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SimulationInterface.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SimulationInterface.cpp

    ${CMAKE_CURRENT_LIST_DIR}/SponzaScene.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SponzaScene.cpp
    ${CMAKE_CURRENT_LIST_DIR}/LightingInterface.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LightingInterface.cpp
)

target_link_libraries(Demo PRIVATE LglEng)
target_link_libraries(Demo PRIVATE LglDeps)