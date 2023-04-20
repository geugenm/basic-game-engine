include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG a89590b4259b91de57f8417109f8ebc299bdb5ad
)

if(imgui_ADDED)
  add_library(imgui SHARED)
  target_sources(imgui PRIVATE
          "${imgui_SOURCE_DIR}/imgui.cpp"
          "${imgui_SOURCE_DIR}/imgui_demo.cpp"
          "${imgui_SOURCE_DIR}/imgui_draw.cpp"
          "${imgui_SOURCE_DIR}/imgui_tables.cpp"
          "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
          )

  target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
  target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}/backends")

  if(WIN32)
    target_sources(imgui PRIVATE
            "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
            )
    target_link_libraries(imgui PUBLIC "d3dcompiler" "d3d11" "Dwmapi" "Imm32")
  else()
    target_sources(imgui PRIVATE
            "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
            )
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
    find_package(OpenGL REQUIRED)
    target_link_libraries(imgui PUBLIC OpenGL::GL)
    target_link_libraries(imgui PUBLIC SDL3::SDL3)
  endif()

endif()
