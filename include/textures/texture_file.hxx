#pragma once

#include "abstract_file.hxx"
#include "color.hxx"

#include <fstream>
#include <regex>
#include <vector>

class TextureFile : public File {
public:
    explicit TextureFile(const std::filesystem::path& file_path)
        : File(file_path) {
        set_path(file_path);
    }

    void load() override {
        // Open the input file stream and enable the fail bit exception
        std::ifstream in_file(get_path(), std::ios_base::binary);
        if (!in_file) {
            throw std::runtime_error("Failed to open input file: " +
                                     get_path().string());
        }
        in_file.exceptions(std::ios_base::failbit);

        // Read and parse the header of the PPM file
        std::string magic_number;
        int         width, height, max_color_value;
        in_file >> magic_number >> width >> height >> max_color_value;
        if (magic_number != "P6") {
            throw std::runtime_error("Unsupported image format: " +
                                     magic_number);
        }
        if (width <= 0 || height <= 0 || max_color_value != 255) {
            throw std::runtime_error("Invalid image metadata");
        }

        width_ = static_cast<size_t>(width);
        height_ = static_cast<size_t>(height);

        const std::streamsize expected_data_size =
            width_ * height_ * sizeof(Color);
        if (!in_file.seekg(0, std::ios::end) ||
            in_file.tellg() - static_cast<std::streamoff>(expected_data_size) !=
                static_cast<std::streamoff>(in_file.beg)) {
            throw std::runtime_error("Invalid image data size");
        }
        in_file.seekg(0, std::ios::beg);

        pixels_.resize(width_ * height_);
        in_file.read(reinterpret_cast<char*>(pixels_.data()),
                     expected_data_size);
        if (!in_file) {
            throw std::runtime_error("Error reading image data");
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

        out_file << "P6\n" << width_ << ' ' << height_ << '\n' << "255\n";

        const auto buffer_size =
            static_cast<std::streamsize>(sizeof(Color) * pixels_.size());
        const char* written_data =
            reinterpret_cast<const char*>(pixels_.data());
        out_file.write(written_data, buffer_size);

        if (out_file.bad()) {
            throw std::invalid_argument("Failed to write to '" +
                                        get_path().string() + "'");
        }
    }

    void save_as(const std::filesystem::path& new_file_path) override { }

    [[nodiscard]] Color get_pixel(const size_t& x, const size_t& y) const {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        return pixels_[y * width_ + x];
    }

    [[nodiscard]] size_t get_width() const { return width_; }

    [[nodiscard]] size_t get_height() const { return height_; }

    void set_pixel(const size_t& x, const size_t& y, const Color& color) {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        pixels_[y * width_ + x] = color;
    }

    void set_dimensions(size_t width, size_t height) {
        pixels_.resize(width * height);
        width_  = width;
        height_ = height;
    }

    void set_pixels(const std::vector<Color>& pixels) {
        if (pixels.size() != width_ * height_) {
            throw std::invalid_argument(
                "Error: The number of pixels doesn't match the dimensions.");
        }

        pixels_ = pixels;
    }

    [[nodiscard]] std::vector<Color> get_pixels() const { return pixels_; }

private:
    std::vector<Color> pixels_;

    std::size_t width_  = 0;
    std::size_t height_ = 0;
};
