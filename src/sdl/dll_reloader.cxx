#include "apps/application.h"

#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>

class

    Application*
    reloadApplicationLibrary(const Application* currentApplication,
        void*& currentHandle)
{
    try {
        if (currentApplication != nullptr) {
            SDL_UnloadObject(currentHandle);
        }

        const std::filesystem::path tempLibraryPath = "apps/5678.so";
        if (std::filesystem::exists(tempLibraryPath)) {
            if (remove(tempLibraryPath) != 0) {
                const std::filesystem::path currentPath = std::filesystem::current_path();
                throw std::runtime_error(
                    "Error: can't remove file: " + tempLibraryPath.string() + ", current path: " + currentPath.string());
            }
        }

        const std::filesystem::path newLibraryPath = "apps/1234.so";
        copy(newLibraryPath, tempLibraryPath);

        void* gameHandle = SDL_LoadObject(tempLibraryPath.c_str());

        if (gameHandle == nullptr) {
            throw std::runtime_error("Error: failed to load dynamic library: " + tempLibraryPath.string() + ", " + SDL_GetError());
        }

        currentHandle = gameHandle;

        SDL_FunctionPointer createGameFuncPtr = SDL_LoadFunction(gameHandle, "createApplication");

        if (createGameFuncPtr == nullptr) {
            throw std::runtime_error(
                "Error: failed to load function 'createApplication' from "
                "dynamic library: "
                + tempLibraryPath.string() + ", " + SDL_GetError());
        }

        using create_game_ptr = decltype(&createApplication);

        auto createGameFunc = reinterpret_cast<create_game_ptr>(createGameFuncPtr);

        Application* newGame = createGameFunc();

        if (newGame == nullptr) {
            throw std::runtime_error(
                "Error: function 'createApplication' returned null pointer");
        }

        return newGame;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}
