#include "apps/application.h"

#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>

void reloadApplicationLibrary(Application* oldGame, void*& oldHandle) {
    if (oldGame != nullptr) {
        SDL_UnloadObject(oldHandle);
    }

    const std::filesystem::path tempLibraryPath = "game/liblib.ExampleGame.so";
    if (std::filesystem::exists(tempLibraryPath)) {
        if (remove(tempLibraryPath) != 0) {
            const std::filesystem::path currentPath = std::filesystem::current_path();
            std::cerr << "Error: can't remove: " << tempLibraryPath
                      << ", current path:" << currentPath << std::endl;
            return;
        }
    }

    const std::filesystem::path newLibraryPath = "game/1234.so";
    try {
        copy(newLibraryPath, tempLibraryPath);
    } catch (const std::exception& exception) {
        std::cerr << "Error: can't copy" << newLibraryPath << " to "
                  << tempLibraryPath;
    }

    void* gameHandle = SDL_LoadObject(tempLibraryPath.c_str());

    if (gameHandle == nullptr) {
        std::cerr << "Error: failed to load: [" << tempLibraryPath << "] "
                  << SDL_GetError() << std::endl;
        return;
    }

    oldHandle = gameHandle;

    SDL_FunctionPointer createGameFuncPtr =
        SDL_LoadFunction(gameHandle, "createApplication");

    if (createGameFuncPtr == nullptr) {
        std::cerr << "error: no function [create_game] in " << tempLibraryPath
                  << " " << SDL_GetError() << std::endl;
        return;
    }

    destroyApplication(oldGame);

    using create_game_ptr = decltype(&createApplication);

    auto createGameFunc = reinterpret_cast<create_game_ptr>(createGameFuncPtr);

    Application* newGame = createGameFunc();

    if (newGame == nullptr) {
        std::cerr << "error: func [create_game] returned: nullptr" << std::endl;
        return;
    }

    oldGame = newGame;
}
