#include "engine/misc/reload_attached_library.hxx"

#include "SDL3/SDL.h"

#include <filesystem>
#include <iostream>

void reloadView(const std::string& libraryName) {
    std::string libraryFinal = "engine/sdl/liblib.SDLView.so";
    void* libraryHandle =
        SDL_LoadObject(libraryFinal.c_str()); // keep handle across calls

    if (libraryHandle != nullptr) {
        SDL_UnloadObject(libraryHandle);
    }

    std::string libraryFinalPath = "engine/sdl/" + libraryName;
    libraryHandle = SDL_LoadObject(libraryFinalPath.c_str());

    if (libraryHandle == nullptr) {
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::cout << "Current path is: " << currentPath.string() << std::endl;
        throw std::runtime_error(std::string("Failed to load library: ") +
                                 SDL_GetError());
    }

    typedef IView* (*createViewFunc)();
    auto createViewPtr = SDL_LoadFunction(libraryHandle, "createView");
    if (createViewPtr == nullptr) {
        throw std::runtime_error(
            std::string("Failed to find symbol createView in library: ") +
            SDL_GetError());
    }
    auto engineFactoryCreateView =
        reinterpret_cast<createViewFunc>(createViewPtr);

    auto view =
        std::shared_ptr<IView>(engineFactoryCreateView(), SDL_UnloadObject);

    Game::Engine::getInstance()->getPresenter()->initialize();

    Game::Engine::getInstance()->getPresenter()->setView(createView());
}
