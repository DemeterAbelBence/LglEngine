cmake_minimum_required(VERSION 4.2.3)

add_library(LglDeps STATIC)

set_target_properties(LglDeps PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
)

set_target_properties(LglDeps PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib"
    OUTPUT_NAME "LglDeps"
)

file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/alldeps/imgui/IMGUI/*.cpp )
file(GLOB IMGUI_HEADERS ${CMAKE_CURRENT_LIST_DIR}/alldeps/imgui/IMGUI/*.h )
source_group("ImGui" FILES ${IMGUI_HEADERS} ${IMGUI_SOURCES})

target_sources(LglDeps PRIVATE 
    ${IMGUI_HEADERS}
    ${IMGUI_SOURCES}
)

target_include_directories(LglDeps PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/imgui
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glew/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glfw/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/assimp/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glm
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/stb
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/xxhash

    ${CMAKE_CURRENT_LIST_DIR}/alldeps/eigen/include/eigen3
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/osqp/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/osqpeigen/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/qdldl/include
)

target_link_libraries(LglDeps PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glfw/lib-vc2022/glfw3.lib
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glew/lib/Release/x64/glew32s.lib
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/assimp/lib/Release/assimp-vc143-mt.lib
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/osqp/lib/Debug/osqp-deb.lib
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/osqpeigen/lib/Debug/OsqpEigen-deb.lib
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/qdldl/lib/Debug/qdldl-deb.lib
    opengl32.lib
)

target_compile_definitions(LglDeps PUBLIC 
    GLEW_STATIC 
    GLEW_STATIC_DEBUG
    GLEW_STATIC_RELEASEDEBUG

    OSQP_EIGEN_OSQP_IS_V1
    OSQP_EIGEN_OSQP_IS_V1_FINAL
)