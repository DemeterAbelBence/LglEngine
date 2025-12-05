add_library(LglDeps STATIC)

set_target_properties(LglDeps PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib"
    OUTPUT_NAME "LglDeps"
)

target_compile_features(LglDeps PUBLIC cxx_std_20)

file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/alldeps/imgui/IMGUI/*.cpp )
file(GLOB IMGUI_HEADERS ${CMAKE_CURRENT_LIST_DIR}/alldeps/imgui/IMGUI/*.h )
source_group("ImGui" FILES ${IMGUI_HEADERS} ${IMGUI_SOURCES})

set(GLMDIR "${CMAKE_CURRENT_LIST_DIR}/alldeps/glm/GLM")

file(GLOB GLM_SOURCE ${GLMDIR}/*.cpp)
file(GLOB GLM_INLINE ${GLMDIR}/*.inl)
file(GLOB GLM_HEADER ${GLMDIR}/*.hpp)
set(GLM ${GLM_SOURCE} ${GLM_INLINE} ${GLM_HEADER})
source_group("Glm" FILES ${GLM})

file(GLOB_RECURSE GLM_DETAIL_SOURCE ${GLMDIR}/detail/*.cpp)
file(GLOB_RECURSE GLM_DETAIL_INLINE ${GLMDIR}/detail/*.inl)
file(GLOB_RECURSE GLM_DETAIL_HEADER ${GLMDIR}/detail/*.hpp)
set(GLM_DETAIL ${GLM_DETAIL_SOURCE} ${GLM_DETAIL_INLINE} ${GLM_DETAIL_HEADER})
source_group("GlmDetail" FILES ${GLM_DETAIL})

file(GLOB_RECURSE GLM_GTC_SOURCE ${GLMDIR}/gtc/*.cpp)
file(GLOB_RECURSE GLM_GTC_INLINE ${GLMDIR}/gtc/*.inl)
file(GLOB_RECURSE GLM_GTC_HEADER ${GLMDIR}/gtc/*.hpp)
set(GLM_GTC ${GLM_GTC_SOURCE} ${GLM_GTC_INLINE} ${GLM_GTC_HEADER})
source_group("GlmGtc" FILES ${GLM_GTC})

file(GLOB_RECURSE GLM_GTX_SOURCE ${GLMDIR}/gtx/*.cpp)
file(GLOB_RECURSE GLM_GTX_INLINE ${GLMDIR}/gtx/*.inl)
file(GLOB_RECURSE GLM_GTX_HEADER ${GLMDIR}/gtx/*.hpp)
set(GLM_GTX ${GLM_GTX_SOURCE} ${GLM_GTX_INLINE} ${GLM_GTX_HEADER})
source_group("GlmGtx" FILES ${GLM_GTX})

set(XXHASH ${CMAKE_CURRENT_LIST_DIR}/alldeps/xxhash/XXHASH/xxhash64.h)
source_group(XxHash FILES ${XXHASH_SOURCES})

set(STB ${CMAKE_CURRENT_LIST_DIR}/alldeps/stb/STB/stb_image.h)
source_group(Stb FILES ${STB_SOURCES})

target_sources(LglDeps PRIVATE 
    ${IMGUI_HEADERS}
    ${IMGUI_SOURCES}

    ${XXHASH}
    ${STB}

    ${GLM}
    ${GLM_DETAIL}
    ${GLM_GTC}
    ${GLM_GTX}
)

target_include_directories(LglDeps PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/imgui
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glew/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glfw/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/assimp/include
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glm
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/stb
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/xxhash
)

target_link_libraries(LglDeps PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glfw/lib-vc2022/glfw3.lib
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/glew/lib/Release/x64/glew32s.lib
    ${CMAKE_CURRENT_LIST_DIR}/alldeps/assimp/lib/Release/assimp-vc143-mt.lib
    opengl32.lib
)

target_compile_definitions(LglDeps PUBLIC 
    GLEW_STATIC 
    GLEW_STATIC_DEBUG
    GLEW_STATIC_RELEASEDEBUG
)