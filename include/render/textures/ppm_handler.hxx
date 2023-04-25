#pragma once

#include "abstract_file.hxx"
#include "render/colors/color_rgb.hxx"

#include <fstream>
#include <regex>
#include <vector>

#include <render/textures/texture.hxx>

class PpmHandler : public File {
public:
    explicit PpmHandler(const std::filesystem::path& file_path)
        : File(file_path),
        texture_(std::make_unique<Texture>()) {}

    void load() override {
        std::ifstream in_file(get_path(), std::ios_base::binary);
        in_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        std::string header, color_format;
        int width, height;

        in_file >> header >> width >> height >> color_format;
        in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Shape2D texture_shape(width, height);
        texture_->set_shape(texture_shape);

        std::vector<ColorRGB> file_read_amount(texture_shape.area());
        in_file.read(reinterpret_cast<char*>(file_read_amount.data()),
                     static_cast<long>(file_read_amount.size() * sizeof(ColorRGB)));

        if (!in_file) {
            throw std::runtime_error("Failed to read image data.");
        }

        texture_->set_pixel_array(file_read_amount);
    }

    void save() override {
        if (get_path().empty()) {
            throw std::invalid_argument("File path is empty");
        }

        std::ofstream out_file(get_path(), std::ios_base::binary);
        out_file.exceptions(std::ios_base::failbit);

        out_file << "P6\n"
                 << texture_->get_shape().width << ' '
                 << texture_->get_shape().height << '\n'
                 << "255\n";

        const auto buffer_size = static_cast<std::streamsize>(
            sizeof(ColorRGB) * texture_->get_pixel_array().size());
        const char* written_data =
            reinterpret_cast<const char*>(texture_->get_pixel_array().data());
        out_file.write(written_data, buffer_size);

        if (!out_file) {
            throw std::invalid_argument("Failed to write to '" +
                                        get_path().string() + "'");
        }
    }

    void save_as(const std::filesystem::path& new_file_path) override {
        const std::filesystem::path saved_path = get_path();
        set_path(new_file_path);
        save();
        set_path(saved_path);
    }

    [[nodiscard]] const Texture& get_texture() const {
        return *texture_;
    }

    void set_texture(std::unique_ptr<Texture> texture) {
        if (!texture) {
            throw std::invalid_argument("Texture pointer is null");
        }
        texture_ = std::move(texture);
    }

    void set_texture(Texture* texture) {
        if (!texture) {
            throw std::invalid_argument("Texture pointer is null");
        }
        texture_.reset(texture);
    }

private:
    std::unique_ptr<Texture> texture_;
};
