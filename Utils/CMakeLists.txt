file(GLOB_RECURSE UTILS_SRC CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE UTILS_HEADERS CONFIGURE_DEPENDS include/Utils/*.h)

add_library(Utils ${UTILS_SRC} ${UTILS_HEADERS})

target_include_directories(Utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)