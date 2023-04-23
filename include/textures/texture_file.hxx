#pragma once

#include "abstract_file.hxx"
#include "color.hxx"

#include <fstream>
#include <vector>
#include <regex>

class TextureFile : public File {
public:
    TextureFile(const std::filesystem::path& file_path)
        : File(file_path) {
        set_path(file_path);
    }

    void load() override {
        std::ifstream in_file(get_path(), std::ios_base::binary);
        in_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        // Read the PAM header from the input file
        std::string header;
        std::getline(in_file, header);
        if (!std::regex_match(header, std::regex("^P7\\s+"))) {
            throw std::invalid_argument("Error: Invalid PAM header.");
        }

        int new_width = 0;
        int new_height = 0;

        // Parse the PAM header fields
        std::string field;
        std::regex field_regex("^([A-Z]+)\\s+(.+)$");
        while (std::getline(in_file, field) && field != "ENDHDR") {
            std::smatch field_match;
            if (std::regex_match(field, field_match, field_regex)) {
                const std::string key = field_match[1];
                const std::string value = field_match[2];
                if (key == "WIDTH") {
                    new_width = std::stoi(value);
                } else if (key == "HEIGHT") {
                    new_height = std::stoi(value);
                }
            }
        }

        // Check that we have read the width and height from the PAM header
        if (new_width <= 0 || new_height <= 0) {
            throw std::invalid_argument("Error: Invalid image dimensions.");
        }

        width_ = static_cast<size_t>(new_width);
        height_ = static_cast<size_t>(new_height);

        // Read the pixel data from the input file
        const auto pixel_count = width_ * height_;
        const auto buf_size = std::streamsize(sizeof(Color) * pixel_count);
        in_file.read(reinterpret_cast<char*>(pixels_.data()), buf_size);

        if (in_file.bad() == true) {
            throw std::invalid_argument("Failed to read file " + get_path().string());
        }
    }

    void save(const std::filesystem::path& file_path) override {
        std::ofstream out_file(file_path, std::ios_base::binary);
        out_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        out_file << "P7\n"
                 << "WIDTH " << width_ << "\n"
                 << "HEIGHT " << height_ << "\n"
                 << "DEPTH 4\n"
                 << "MAXVAL 255\n"
                 << "TUPLTYPE RGB_ALPHA\n"
                 << "ENDHDR\n";

        const auto buffer_size =
            std::streamsize(sizeof(Color) * pixels_.size());

        const char* written_image_pixels =
            reinterpret_cast<const char*>(pixels_.data());
        out_file.write(written_image_pixels, buffer_size);
    }

    [[nodiscard]] Color get_pixel(const size_t & x, const size_t & y) const {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        return pixels_[y * width_ + x];
    }

    [[nodiscard]] size_t get_width() const {
        return width_;
    }

    [[nodiscard]] size_t get_height() const {
        return height_;
    }

    void set_pixel(const size_t & x, const size_t & y, const Color& color) {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        pixels_[y * width_ + x] = color;
    }

    void set_dimensions(size_t width, size_t height) {
        pixels_.resize(width * height);
        width_ = width;
        height_ = height;
    }

    void set_pixels(const std::vector<Color>& pixels) {
        if (pixels.size() != width_ * height_) {
            throw std::invalid_argument("Error: The number of pixels doesn't match the dimensions.");
        }

        pixels_ = pixels;
    }

private:
    std::vector<Color> pixels_;

    std::size_t width_  = 0;
    std::size_t height_ = 0;
};
