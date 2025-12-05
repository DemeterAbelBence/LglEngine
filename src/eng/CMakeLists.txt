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
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    ${CMAKE_CURRENT_LIST_DIR}/engstd.mpp

PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/eng/Scene.hpp
    ${CMAKE_CURRENT_LIST_DIR}/eng/Scene.cpp

    ${CMAKE_CURRENT_LIST_DIR}/eng/UserInterface.hpp
    ${CMAKE_CURRENT_LIST_DIR}/eng/UserInterface.cpp

    ${CMAKE_CURRENT_LIST_DIR}/eng/WindowManager.hpp
    ${CMAKE_CURRENT_LIST_DIR}/eng/WindowManager.cpp

    ${CMAKE_CURRENT_LIST_DIR}/eng/CollisionHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/eng/CollisionHandler.cpp
)

target_include_directories(LglEng PUBLIC ${CMAKE_CURRENT_LIST_DIR})