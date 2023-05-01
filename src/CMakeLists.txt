cmake_minimum_required(VERSION 3.25)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages.cmake)

include(${CPM_PACKAGES_DIR}/sdl.cmake)

include(${CPM_PACKAGES_DIR}/glog.cmake)
include(${CPM_PACKAGES_DIR}/tracy.cmake)
include(${CPM_PACKAGES_DIR}/ccache.cmake)


add_library(engine)

file(GLOB_RECURSE RENDER_SOURCES CONFIGURE_DEPENDS "render/*.cxx" "render/*.cpp" "render/*.cc" "render/*.c")
file(GLOB_RECURSE RENDER_HEADERS CONFIGURE_DEPENDS "render/*.h" "render/*.hpp" "render/*.hxx" engine.hxx)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(engine PUBLIC
        ${RENDER_SOURCES}
        )

target_link_libraries(engine PUBLIC engine-includes)

target_compile_features(engine PRIVATE cxx_std_23)
