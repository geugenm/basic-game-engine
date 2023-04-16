#include "engine/misc/reload_attached_library.hxx"

#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>

void reloadLibrary(Game::Engine& engine, const std::string& libraryName) {
    void* libraryHandle = SDL_LoadObject(libraryName.c_str());
    if (libraryHandle == nullptr) {
        throw std::runtime_error(std::string("Failed to load library: ") + SDL_GetError());
    }

    auto engineFactoryCreatePresenterPtr = SDL_LoadFunction(libraryHandle, "createPresenter");
    if (engineFactoryCreatePresenterPtr == nullptr) {
        throw std::runtime_error(std::string("Failed to find symbol createPresenter in library: ") + SDL_GetError());
    }

    auto engineFactoryCreatePresenter = reinterpret_cast<IPresenter* (*)()>(engineFactoryCreatePresenterPtr);
    auto presenter = std::shared_ptr<IPresenter>(engineFactoryCreatePresenter(), SDL_UnloadObject);

    engine.setPresenter(presenter);
}
