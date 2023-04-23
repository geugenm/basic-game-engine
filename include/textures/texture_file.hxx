#pragma once

#include <filesystem>

class File {
public:
    File(const std::filesystem::path & path) {
        set_path(path);
    }

    void set_path(const std::filesystem::path & path) {
        if (exists(path) == false) {
            throw std::invalid_argument("Given texture file path '" + path.string() + "' is not exist");
        }
        path_ = path;
    }

    [[nodiscard]] std::filesystem::path get_path() const {
        return path_;
    }

    virtual void load() = 0;
    virtual void save() = 0;

private:
    std::filesystem::path path_;
};
