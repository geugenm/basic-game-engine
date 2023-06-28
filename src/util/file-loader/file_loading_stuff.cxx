#include "file_loading_stuff.hxx"

namespace sdk
{

suppl::membuf suppl::load_file(std::string_view path)
{
    SDL_RWops *io = SDL_RWFromFile(path.data(), "rb");
    if (nullptr == io)
    {
        throw std::invalid_argument("Can't load file: " + std::string(path));
    }

    Sint64 file_size = io->size(io);
    if (-1 == file_size)
    {
        throw std::invalid_argument("can't determine size of file: " +
                                    std::string(path));
    }
    const auto size = static_cast<size_t>(file_size);
    auto mem        = std::make_unique<char[]>(size);

    const size_t num_readed_objects = io->read(io, mem.get(), size);
    if (num_readed_objects != size)
    {
        throw std::invalid_argument("can't read all content from file: " +
                                    std::string(path));
    }

    if (0 != io->close(io))
    {
        throw std::invalid_argument("failed close file: " + std::string(path));
    }
    return membuf(std::move(mem), size);
}
} // namespace sdk