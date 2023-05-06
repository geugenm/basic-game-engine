cmake_minimum_required(VERSION 3.14)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages.cmake)

if (NOT TARGET SDL3::SDL3-static OR NOT TARGET SDL3::SDL3-shared)
    message(FATAL_ERROR "SDL3 targets not found. Please make sure SDL3 is installed and properly linked.")
endif ()

add_executable(Test_SDL_Static_Compilation sdl_general.cxx)
target_link_libraries(Test_SDL_Static_Compilation PRIVATE GTest::GTest SDL3::SDL3-static)

add_executable(Test_SDL_Shared_Compilation sdl_general.cxx)
target_link_libraries(Test_SDL_Shared_Compilation PRIVATE GTest::GTest SDL3::SDL3-shared)

set(SHARED_LIB_PATH ${CMAKE_CURRENT_LIST_DIR}/dll)
option(USE_CUSTOM_SHARED_LIB_PATH "Use a custom shared library search path" OFF)

if (USE_CUSTOM_SHARED_LIB_PATH AND SHARED_LIB_PATH)
    add_executable(Test_Custom_Dynamic_API sdl_general.cxx)
    target_link_libraries(Test_Custom_Dynamic_API PRIVATE GTest::GTest SDL3::SDL3-shared)

    add_custom_command(TARGET Test_Custom_Dynamic_API POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E env SDL3_DYNAMIC_API=${SHARED_LIB_PATH}/libSDL2.so $<TARGET_FILE_DIR:Test_Custom_Dynamic_API>/./Test_Custom_Dynamic_API
            )
endif ()

option(USE_CUSTOM_SDL_SHARED_LIB "Use custom SDL shared libraries. NOTE: Will fail if something was injected" OFF)

if (USE_CUSTOM_SDL_SHARED_LIB AND SHARED_LIB_PATH)
    add_executable(Test_DLL_Injection sdl_general.cxx)
    target_link_libraries(Test_DLL_Injection PRIVATE GTest::GTest SDL3::SDL3-shared)

    add_custom_command(TARGET Test_DLL_Injection POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E env LD_PRELOAD=${SHARED_LIB_PATH}/libSDL2.so $<TARGET_FILE_DIR:Test_DLL_Injection>/./Test_DLL_Injection
            )
endif ()

add_executable(Test_OpenGL_Init opengl_general.cxx)
target_link_libraries(Test_OpenGL_Init PRIVATE opengl-glad GTest::GTest SDL3::SDL3-shared)

add_executable(Test_Engine_Shaders_SDL engine_shader_test.cxx)
target_link_libraries(Test_Engine_Shaders_SDL PRIVATE opengl-glad sdl-engine GTest::GTest SDL3::SDL3-shared)
