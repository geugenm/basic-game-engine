#include "engine/misc/reload_attached_library.hxx"

#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>

void reloadView(const std::string& libraryName) {
    std::string libraryFinalPath = "engine/sdl/" + libraryName;
    void* libraryHandle = SDL_LoadObject(libraryFinalPath.c_str());

    if (libraryHandle == nullptr) {
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::cout << "Current path is: " << currentPath.string() << std::endl;
        throw std::runtime_error(std::string("Failed to load library: ") + SDL_GetError());
    }

    auto createViewPtr = SDL_LoadFunction(libraryHandle, "createView");
    if (createViewPtr == nullptr) {
        throw std::runtime_error(std::string("Failed to find symbol createPresenter in library: ") + SDL_GetError());
    }

    auto engineFactoryCreateView = reinterpret_cast<IView* (*)()>(createViewPtr);
    auto view = std::shared_ptr<IView>(engineFactoryCreateView(), SDL_UnloadObject);

    Game::Engine::getInstance()->getPresenter()->setView(view);
}
