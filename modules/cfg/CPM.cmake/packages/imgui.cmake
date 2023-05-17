include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG v1.88
        DOWNLOAD_ONLY True
)


if (imgui_ADDED)
  add_library(imgui STATIC)
  target_sources(imgui PRIVATE
          "${imgui_SOURCE_DIR}/imgui.cpp"
          "${imgui_SOURCE_DIR}/imgui_demo.cpp"
          "${imgui_SOURCE_DIR}/imgui_draw.cpp"
          "${imgui_SOURCE_DIR}/imgui_tables.cpp"
          "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
          )

  target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
endif ()
