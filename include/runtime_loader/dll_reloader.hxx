#pragma once

class IDLLReloader {
public:
    virtual ~IDLLReloader() = default;

    virtual void set_target_library_path(const char* path) = 0;

    virtual void set_new_library_path(const char* path) = 0;

    virtual void call_external_method_by_name(const char* method_name) = 0;

    virtual void reload_library() = 0;

    virtual void reset() = 0;
};

extern "C" IDLLReloader* create_dll_reloader();
extern "C" void destroy_dll_reloader(IDLLReloader * reloader);
