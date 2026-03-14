cmake_minimum_required(VERSION 4.2.3)

add_subdirectory(mod)
add_subdirectory(util)

target_link_libraries(LglMod PUBLIC LglDeps)
target_link_libraries(LglUtil PUBLIC LglMod)

add_subdirectory(core)
target_link_libraries(LglCore PUBLIC LglUtil)

add_subdirectory(eng)
target_link_libraries(LglEng PUBLIC LglCore)