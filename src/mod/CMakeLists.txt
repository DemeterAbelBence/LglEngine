cmake_minimum_required(VERSION 4.2.3)

add_library(LglMod STATIC)

set_target_properties(LglMod PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
)

target_sources(LglMod PRIVATE
 FILE_SET cxx_modules TYPE CXX_MODULES FILES
    ${CMAKE_CURRENT_LIST_DIR}/lglmodule.ixx
    ${CMAKE_CURRENT_LIST_DIR}/glmmodule.ixx
    ${CMAKE_CURRENT_LIST_DIR}/asimmodule.ixx
    ${CMAKE_CURRENT_LIST_DIR}/qpmodule.ixx
)
