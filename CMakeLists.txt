cmake_minimum_required(VERSION 4.2.3)

project(LightGraphicsLibraryEngine LANGUAGES CXX)

add_subdirectory(dep)
add_subdirectory(src)
add_subdirectory(lgl_demo)

file (
    COPY 
        "${CMAKE_CURRENT_LIST_DIR}/dep/alldeps/assimp/bin/assimp-vc143-mt.dll" 
    DESTINATION "${CMAKE_BINARY_DIR}/../bin/Debug"
)

#file (
#    COPY 
#        "${CMAKE_CURRENT_LIST_DIR}/dep/alldeps/osqp/bin/osqp.dll" 
#    DESTINATION "${CMAKE_BINARY_DIR}/../bin/Debug"
#)
#
#file (
#    COPY 
#        "${CMAKE_CURRENT_LIST_DIR}/dep/alldeps/osqpeigen/bin/OsqpEigen.dll" 
#    DESTINATION "${CMAKE_BINARY_DIR}/../bin/Debug"
#)
#
#file (
#    COPY 
#        "${CMAKE_CURRENT_LIST_DIR}/dep/alldeps/qdldl/bin/qdldl.dll" 
#    DESTINATION "${CMAKE_BINARY_DIR}/../bin/Debug"
#)

file (
    COPY 
        "${CMAKE_CURRENT_LIST_DIR}/res/assets"
        "${CMAKE_CURRENT_LIST_DIR}/res/imgui/imgui.ini"
    DESTINATION "${CMAKE_BINARY_DIR}/lgl_demo"
)

file (
    COPY 
        "${CMAKE_CURRENT_LIST_DIR}/res/assets"
        "${CMAKE_CURRENT_LIST_DIR}/res/imgui/imgui.ini"
    DESTINATION "${CMAKE_BINARY_DIR}/../bin/Debug"
)