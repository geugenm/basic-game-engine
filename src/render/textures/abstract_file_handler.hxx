#pragma once

#include <filesystem>

class File
{
public:
    explicit File(const std::filesystem::path& file_path)
    {
        set_path(file_path);
    }

    virtual ~File() = default;

    void set_path(const std::filesystem::path& file_path)
    {
        path_ = file_path;
    }

    [[nodiscard]] std::filesystem::path get_path() const
    {
        return path_;
    }

    virtual void load()                                              = 0;
    virtual void save()                                              = 0;
    virtual void save_as(const std::filesystem::path& new_file_path) = 0;

private:
    std::filesystem::path path_;
};
