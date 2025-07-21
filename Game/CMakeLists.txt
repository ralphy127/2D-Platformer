# game/CMakeLists.txt

file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS include/Game/*.h)

add_executable(Game
    Main.cpp
    ${GAME_SRC}
    ${GAME_HEADERS})

target_include_directories(Game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Game
    PRIVATE
        Engine
        Utils
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Game/assets
    $<TARGET_FILE_DIR:Game>/assets
)
