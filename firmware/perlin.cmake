# SRC: https://github.com/lemariva/micropython-camera-driver/blob/master/src/micropython.cmake
add_library(usermod_perlin INTERFACE)

target_sources(usermod_perlin INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/perlin.c
)

target_include_directories(usermod_perlin INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_perlin INTERFACE)

target_link_libraries(usermod INTERFACE usermod_perlin)

