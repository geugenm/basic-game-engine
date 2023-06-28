package mil.eugene.gamedev;

import org.libsdl.app.SDLActivity;

public class GameDevSimulator extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[]{
                "c++_shared",
                "stb_image",
                "opengl_utils",
                "SDL3",
                "sdl3_utils",
                "imgui",
                "imgui_utils",
                "sdk_components",
                "player_system",
                "sdk_systems",
                "engine_sdk",
                "test_engine_android"
        };
    }
}
