cmake_minimum_required(VERSION 4.2.3)

add_library(LglEng STATIC)

if (MSVC)
    target_compile_options(LglEng PUBLIC
        /W4            # High warning level
        /WX-           # Do not treat warnings as errors (change to /WX if you want strict)
        /permissive-   # Enforce standard C++ compliance
        /EHsc          # Enable standard exception handling
        /MP            # Multi-processor compilation
        /Zi            # Debug info
    )
endif(MSVC)

set_target_properties(LglEng PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_target_properties(LglEng PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib"
    OUTPUT_NAME "LglEng"
)

target_sources(LglEng 
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/eng/Scene.hxx
    ${CMAKE_CURRENT_LIST_DIR}/eng/Scene.cxx

    ${CMAKE_CURRENT_LIST_DIR}/eng/UserInterface.hxx
    ${CMAKE_CURRENT_LIST_DIR}/eng/UserInterface.cxx

    ${CMAKE_CURRENT_LIST_DIR}/eng/WindowManager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/eng/WindowManager.cxx

    ${CMAKE_CURRENT_LIST_DIR}/eng/CollisionHandler.hxx
    ${CMAKE_CURRENT_LIST_DIR}/eng/CollisionHandler.cxx

    ${CMAKE_CURRENT_LIST_DIR}/eng/SceneParser.hxx
    ${CMAKE_CURRENT_LIST_DIR}/eng/SceneParser.cxx
)

target_include_directories(LglEng PUBLIC ${CMAKE_CURRENT_LIST_DIR})