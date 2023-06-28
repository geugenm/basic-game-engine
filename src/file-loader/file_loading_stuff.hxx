#pragma once

#include <SDL3/SDL_rwops.h>
#include <nlohmann/json.hpp>
#include <streambuf>

namespace sdk::suppl
{
[[nodiscard]] static nlohmann::json
get_file_json_content(std::filesystem::path file_path)
{
    static constexpr std::string_view properties_extension = ".json";

    file_path.replace_extension(properties_extension);

    std::string file_path_string = file_path;

    if (!file_path_string.empty() && file_path_string[0] == '/')
    {
        file_path_string.erase(0, 1); // remove the first character
    }

    SDL_RWops *rw = SDL_RWFromFile(file_path_string.c_str(), "rb");
    if (rw == nullptr)
    {
        throw std::invalid_argument("Unable to open file: " + file_path_string +
                                    ", SDL says: " + SDL_GetError());
    }

    static constexpr Sint64 buffer_size = 2048;
    char buffer[buffer_size];
    const size_t read_count = SDL_RWread(rw, buffer, buffer_size - 1);
    if (read_count == 0)
    {
        throw std::invalid_argument(
            "Error reading from file: " + file_path_string +
            ", SDL says: " + SDL_GetError());
    }

    buffer[read_count] = '\0';

    SDL_RWclose(rw);

    return nlohmann::json::parse(buffer);
}

struct membuf : public std::streambuf
{
    membuf() : std::streambuf(), buf(), buf_size(0) {}
    membuf(std::unique_ptr<char[]> buffer, size_t size)
        : std::streambuf(), buf(std::move(buffer)), buf_size(size)
    {
        char *beg_ptr = buf.get();
        char *end_ptr = beg_ptr + buf_size;
        setg(beg_ptr, beg_ptr, end_ptr);
        setp(beg_ptr, end_ptr);
    }
    membuf(membuf &&other)
    {
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);

        other.swap(*this);

        buf      = std::move(other.buf);
        buf_size = other.buf_size;

        other.buf_size = 0;
    }

    pos_type seekoff(off_type pos, std::ios_base::seekdir seek_dir,
                     std::ios_base::openmode) override
    {
        // TODO implement it in correct way
        if (seek_dir == std::ios_base::beg)
        {
            return 0 + pos;
        }
        else if (seek_dir == std::ios_base::end)
        {
            return buf_size + pos;
        }
        else
        {
            return egptr() - gptr();
        }
    }

    char *begin() const
    {
        return eback();
    }
    size_t size() const
    {
        return buf_size;
    }

private:
    std::unique_ptr<char[]> buf;
    size_t buf_size;
};

membuf load_file(std::string_view path);

[[nodiscard]] static std::string
get_file_content(std::filesystem::path file_path)
{
    std::string file_path_string = file_path;

    if (!file_path_string.empty() && file_path_string[0] == '/')
    {
        file_path_string.erase(0, 1); // remove the first character
    }

    SDL_RWops *rw = SDL_RWFromFile(file_path_string.c_str(), "rb");
    if (rw == nullptr)
    {
        throw std::invalid_argument("Unable to open file: " + file_path_string +
                                    ", SDL says: " + SDL_GetError());
    }

    static constexpr Sint64 buffer_size = 2048;
    char buffer[buffer_size];
    std::stringstream ss; // create a stringstream
    size_t read_count;
    while ((read_count = SDL_RWread(rw, buffer, buffer_size - 1)) > 0)
    {
        buffer[read_count] = '\0';
        ss << buffer; // add buffer content to the stringstream
    }

    if (SDL_RWclose(rw) != 0)
    {
        throw std::invalid_argument(
            "Error reading from file: " + file_path_string +
            ", SDL says: " + SDL_GetError());
    }

    std::string file_content = ss.str();

    return file_content;
}
} // namespace sdk::suppl