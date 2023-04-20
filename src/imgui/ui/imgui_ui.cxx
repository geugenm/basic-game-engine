#include "imgui_ui.hxx"

AbstractUI* create_renderer() {
    return new ImGuiUI();
}

void destroy_renderer(ImGuiUI* renderer) {
    delete renderer;
}
