add_subdirectory(core)
add_subdirectory(eng)
add_subdirectory(util)

target_link_libraries(LglUtil PUBLIC LglDeps)
target_link_libraries(LglCore PUBLIC LglUtil)
target_link_libraries(LglEng PUBLIC LglCore)