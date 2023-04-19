#ifndef BASIC_GAME_ENGINE_RELOADER_H
#define BASIC_GAME_ENGINE_RELOADER_H

struct DLLReloader {
    virtual ~DLLReloader() = default;

    virtual void set_target_library_path(const char* path) = 0;

    virtual void set_new_library_path(const char* path) = 0;

    virtual void set_current_library_handle(void*& handle) = 0;

    virtual void call_external_method_by_name(const char* method_name) = 0;

    virtual void reload_library() = 0;

    virtual void reset() = 0;
};


extern "C" DLLReloader * create_dll_reloader();
extern "C" void destroy_dll_reloader();


#endif // BASIC_GAME_ENGINE_RELOADER_H
