cmake_minimum_required(VERSION 3.21)

project(LightGraphicsLibraryEngine LANGUAGES CXX)

add_subdirectory(dep)
add_subdirectory(src)
add_subdirectory(lgl_demo)

file (
    COPY 
        "${CMAKE_CURRENT_LIST_DIR}/dep/alldeps/assimp/assimp-vc143-mt.dll" 
    DESTINATION "${CMAKE_BINARY_DIR}/../bin/Debug"
)

file (
    COPY 
        "${CMAKE_CURRENT_LIST_DIR}/res/assets"
        "${CMAKE_CURRENT_LIST_DIR}/res/shaders" 
        "${CMAKE_CURRENT_LIST_DIR}/res/imgui/imgui.ini"
    DESTINATION "${CMAKE_BINARY_DIR}/lgl_demo"
)

file (
    COPY 
        "${CMAKE_CURRENT_LIST_DIR}/res/assets"
        "${CMAKE_CURRENT_LIST_DIR}/res/shaders"
        "${CMAKE_CURRENT_LIST_DIR}/res/imgui/imgui.ini"
    DESTINATION "${CMAKE_BINARY_DIR}/../bin/Debug"
)