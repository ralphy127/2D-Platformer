file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS include/Engine/*.h)

add_library(Engine ${ENGINE_SRC} ${ENGINE_HEADERS})

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Engine
    PUBLIC
        Utils
)
