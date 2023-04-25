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
        : File(file_path) {
        set_path(file_path);
        texture_ = std::make_unique<Texture>();
    }

    void load() override {
        std::ifstream in_file(get_path(), std::ios_base::binary);
        in_file.exceptions(std::ios_base::failbit);

        std::string header, color_format;
        int         width, height;

        in_file >> header >> width >> height >> color_format;

        if (in_file.get() != '\n') {
            throw std::runtime_error("Expected newline after image metadata.");
        }

        texture_->set_dimensions(static_cast<size_t>(width),
                                 static_cast<size_t>(height));


        in_file.read(reinterpret_cast<char*>(texture_->get_pixel_array().data()),
                     static_cast<long>(texture_->get_pixel_array().size() * sizeof(ColorRGB)));

        if (in_file.bad()) {
            throw std::runtime_error("Failed to read image data.");
        }
    }

    void save() override {
        if (get_path().empty()) {
            throw std::invalid_argument("File path is empty");
        }

        std::ofstream out_file(get_path().string(), std::ios_base::binary);
        if (!out_file) {
            throw std::runtime_error("Failed to open output file: " +
                                     get_path().string());
        }
        out_file.exceptions(std::ios_base::failbit);

        out_file << "P6\n" << texture_->get_width() << ' ' << texture_->get_height() << '\n' << "255\n";

        const auto buffer_size =
            static_cast<std::streamsize>(sizeof(ColorRGB) * texture_->get_pixel_array().size());
        const char* written_data =
            reinterpret_cast<const char*>(texture_->get_pixel_array().data());
        out_file.write(written_data, buffer_size);

        if (out_file.bad()) {
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

    [[nodiscard]] Texture & get_texture() const {
        return *texture_;
    }

private:
    std::unique_ptr<Texture> texture_;
};
