#pragma once

#include "../colors/color_rgb.hxx"
#include "abstract_file_handler.hxx"

#include <fstream>
#include <regex>
#include <vector>

#include "texture.hxx"

class PpmHandler : public File
{
  public:
    explicit PpmHandler(const std::filesystem::path &file_path) : File(file_path), texture_(std::make_unique<Texture>())
    {
    }

    explicit PpmHandler(const std::filesystem::path &file_path, const Texture &texture)
        : File(file_path), texture_(std::make_unique<Texture>())
    {
        set_texture(texture);
    }

    PpmHandler(const PpmHandler &other) : File(other), texture_(std::make_unique<Texture>(other.get_texture()))
    {
    }

    PpmHandler(PpmHandler &&other) noexcept : File(other), texture_(std::move(other.texture_))
    {
        other.texture_ = nullptr;
    }

    PpmHandler &operator=(const PpmHandler &other)
    {
        if (this != &other)
        {
            File::operator=(other);
            texture_ = std::make_unique<Texture>(other.get_texture());
        }
        return *this;
    }

    PpmHandler &operator=(PpmHandler &&other) noexcept
    {
        if (this != &other)
        {
            File::operator=(other);
            texture_ = std::move(other.texture_);
            other.texture_ = nullptr;
        }
        return *this;
    }

    ~PpmHandler() override = default;

    void load() override
    {
        std::ifstream in_file(get_path(), std::ios_base::binary);
        in_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        try
        {
            std::string header, color_format;
            int width, height;

            in_file >> header >> width >> height >> color_format;
            in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            BoundingBox texture_shape(width, height);
            texture_->set_shape(texture_shape);

            std::vector<ColorRGB> file_read_amount(texture_shape.area());
            in_file.read(reinterpret_cast<char *>(file_read_amount.data()),
                         static_cast<long>(file_read_amount.size() * sizeof(ColorRGB)));

            if (!in_file)
            {
                throw std::runtime_error("Failed to read image data.");
            }

            texture_->set_pixel_array(file_read_amount);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Failed to load PPM file '" + get_path().string() + "': " + e.what());
        }
    }

    void save() override
    {
        if (get_path().empty())
        {
            throw std::invalid_argument("File path is empty");
        }

        if (exists(get_path()))
        {
            std::filesystem::remove(get_path());
        }

        if (!texture_)
        {
            throw std::invalid_argument("Trying to save empty texture");
        }

        std::ofstream out_file(get_path(), std::ios_base::binary | std::ios::out | std::ios::app);
        out_file.exceptions(std::ios_base::failbit);

        try
        {
            out_file << "P6\n" << texture_->get_shape().width << ' ' << texture_->get_shape().height << '\n' << "255\n";

            const auto buffer_size =
                static_cast<std::streamsize>(sizeof(ColorRGB) * texture_->get_pixel_array().size());
            const char *written_data = reinterpret_cast<const char *>(texture_->get_pixel_array().data());
            out_file.write(written_data, buffer_size);

            if (!out_file)
            {
                throw std::runtime_error("Failed to write to file '" + get_path().string() + "'");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Failed to save PPM file '" + get_path().string() + "': " + e.what());
        }
    }

    void save_as(const std::filesystem::path &new_file_path) override
    {
        const std::filesystem::path saved_path = get_path();
        set_path(new_file_path);
        save();
        set_path(saved_path);
    }

    [[nodiscard]] const Texture &get_texture() const
    {
        if (!texture_)
        {
            throw std::runtime_error("Texture pointer is null");
        }
        return *texture_;
    }

    void set_texture(const Texture &texture)
    {
        texture_ = std::make_unique<Texture>(texture);
    }

  private:
    std::unique_ptr<Texture> texture_;
};
