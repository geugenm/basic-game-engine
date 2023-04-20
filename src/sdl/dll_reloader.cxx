#include "runtime_loader/dll_reloader.hxx"

#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>

class SDLDLLReloader : public IDLLReloader {
public:
    SDLDLLReloader() { }

    ~SDLDLLReloader() override = default;

    void set_target_library_path(const char* path) override
    {
        if (std::filesystem::exists(path) == false) {
            throw std::invalid_argument("Target library does not exist: " + target_library_path_.string());
        }
        target_library_path_ = path;
    }

    void set_new_library_path(const char* path) override
    {
        if (std::filesystem::exists(path) == false) {
            throw std::invalid_argument("Target library does not exist: " + target_library_path_.string());
        }

        new_library_path_ = path;
    }

    void call_external_method_by_name(const char* method_name) override
    {
        get_sdl_function_pointer(method_name)();
    }

    void reload_library() override
    {
    }

    void reset() override
    {
    }

    [[nodiscard]] SDL_FunctionPointer get_sdl_function_pointer(const std::string_view& method_name)
    {
        void* gameHandle = SDL_LoadObject(new_library_path_.c_str());

        if (gameHandle == nullptr) {
            throw std::runtime_error("Error: failed to load dynamic library: " + new_library_path_.string() + ", " + SDL_GetError());
        }

        SDL_FunctionPointer createGameFuncPtr = SDL_LoadFunction(gameHandle, method_name.data());

        if (createGameFuncPtr == nullptr) {
            throw std::runtime_error(
                "Error: failed to load function 'create_application' from "
                "dynamic library: "
                + new_library_path_.string() + ", " + SDL_GetError());
        }

        return createGameFuncPtr;
    }

private:
    std::filesystem::path target_library_path_;

    std::filesystem::path new_library_path_;

    std::string_view method_name_;
};

IDLLReloader* create_dll_reloader()
{
    return new SDLDLLReloader();
}

void destroy_dll_reloader(IDLLReloader* reloader)
{
    if (reloader == nullptr) {
        return;
    }
    delete reloader;
}
