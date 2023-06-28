#include "sprite.hxx"

namespace sdk
{
suppl::membuf suppl::load_file(std::string_view path)
{
    SDL_RWops *io = SDL_RWFromFile(path.data(), "rb");
    if (nullptr == io)
    {
        throw std::runtime_error("can't load file: " + std::string(path));
    }

    Sint64 file_size = io->size(io);
    if (-1 == file_size)
    {
        throw std::runtime_error("can't determine size of file: " +
                                 std::string(path));
    }
    const size_t size           = static_cast<size_t>(file_size);
    std::unique_ptr<char[]> mem = std::make_unique<char[]>(size);

    const size_t num_readed_objects = io->read(io, mem.get(), size);
    if (num_readed_objects != size)
    {
        throw std::runtime_error("can't read all content from file: " +
                                 std::string(path));
    }

    if (0 != io->close(io))
    {
        throw std::runtime_error("failed close file: " + std::string(path));
    }
    return membuf(std::move(mem), size);
}
} // namespace sdk