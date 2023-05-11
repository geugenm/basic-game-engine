include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG v1.88
)

if (imgui_ADDED)
  add_library(imgui STATIC
          ${imgui_SOURCE_DIR}/imgui.cpp
          ${imgui_SOURCE_DIR}/imgui_demo.cpp # optionally comment this out
          ${imgui_SOURCE_DIR}/imgui_draw.cpp
          ${imgui_SOURCE_DIR}/imgui_widgets.cpp
          ${imgui_SOURCE_DIR}/imgui_tables.cpp
          )
  target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})
  target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS) # optional imgui setting
  set_target_properties(imgui PROPERTIES FOLDER third-party) # optoinal IDE dir


  add_library(imgui_sdl STATIC
          ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
          ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.cpp
          )

  target_link_libraries(imgui_sdl PRIVATE imgui SDL)
  target_include_directories(imgui_sdl INTERFACE ${imgui_SOURCE_DIR}/backends)
endif ()
