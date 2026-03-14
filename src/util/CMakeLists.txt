cmake_minimum_required(VERSION 4.2.3)

add_library(LglUtil STATIC)

set_target_properties(LglUtil PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
)

set_target_properties(LglUtil PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib"
    OUTPUT_NAME "LglUtil"
)

target_sources(LglUtil PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/stb_image.cpp

    ${CMAKE_CURRENT_LIST_DIR}/gl/Debug.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/Debug.hxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/DebugDrawer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/DebugDrawer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/FrameBuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/FrameBuffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/GpuProgram.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/GpuProgram.hxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/Texture.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/Texture.hxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/VertexArray.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/VertexArray.hxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/VertexBuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/VertexBuffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/VertexBufferLayout.cxx
    ${CMAKE_CURRENT_LIST_DIR}/gl/VertexBufferLayout.hxx

    ${CMAKE_CURRENT_LIST_DIR}/glfw/keyin.cxx
    ${CMAKE_CURRENT_LIST_DIR}/glfw/keyin.hxx
    ${CMAKE_CURRENT_LIST_DIR}/glfw/Mouse.cxx
    ${CMAKE_CURRENT_LIST_DIR}/glfw/Mouse.hxx
    ${CMAKE_CURRENT_LIST_DIR}/glfw/Time.cxx
    ${CMAKE_CURRENT_LIST_DIR}/glfw/Time.hxx

    ${CMAKE_CURRENT_LIST_DIR}/math/Camera.cxx
    ${CMAKE_CURRENT_LIST_DIR}/math/Camera.hxx
    ${CMAKE_CURRENT_LIST_DIR}/math/Dnum.cxx
    ${CMAKE_CURRENT_LIST_DIR}/math/Dnum.hxx
    ${CMAKE_CURRENT_LIST_DIR}/math/Quaternion.cxx
    ${CMAKE_CURRENT_LIST_DIR}/math/Quaternion.hxx
    ${CMAKE_CURRENT_LIST_DIR}/math/RigidBody.cxx
    ${CMAKE_CURRENT_LIST_DIR}/math/RigidBody.hxx
    ${CMAKE_CURRENT_LIST_DIR}/math/Transformation.cxx
    ${CMAKE_CURRENT_LIST_DIR}/math/Transformation.hxx

    ${CMAKE_CURRENT_LIST_DIR}/model/model.cxx
    ${CMAKE_CURRENT_LIST_DIR}/model/model.hxx
)
target_include_directories(LglUtil PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)