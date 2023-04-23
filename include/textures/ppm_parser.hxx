#pragma once

#include "abstract_file.hxx"
#include "color_rgb.hxx"

#include <fstream>
#include <regex>
#include <vector>

class PpmParser : public File {
public:
    explicit PpmParser(const std::filesystem::path& file_path)
        : File(file_path) {
        set_path(file_path);
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

        set_dimensions(static_cast<size_t>(width), static_cast<size_t>(height));

        pixels_.resize(width_ * height_);

        in_file.read(reinterpret_cast<char*>(pixels_.data()),
                     static_cast<long>(pixels_.size() * sizeof(ColorRGB)));

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

        out_file << "P6\n" << width_ << ' ' << height_ << '\n' << "255\n";

        const auto buffer_size =
            static_cast<std::streamsize>(sizeof(ColorRGB) * pixels_.size());
        const char* written_data =
            reinterpret_cast<const char*>(pixels_.data());
        out_file.write(written_data, buffer_size);

        if (out_file.bad()) {
            throw std::invalid_argument("Failed to write to '" +
                                        get_path().string() + "'");
        }
    }

    void save_as(const std::filesystem::path& new_file_path) override { }

    [[nodiscard]] ColorRGB get_pixel(const size_t& x, const size_t& y) const {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        return pixels_[y * width_ + x];
    }

    [[nodiscard]] size_t get_width() const { return width_; }

    [[nodiscard]] size_t get_height() const { return height_; }

    void set_pixel(const size_t& x, const size_t& y, const ColorRGB& color) {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        pixels_[y * width_ + x] = color;
    }

    void set_dimensions(size_t width, size_t height) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument(
                "Given 'height' or 'width' value is <= 0.");
        }
        pixels_.resize(width * height);
        width_  = width;
        height_ = height;
    }

    void set_pixels(const std::vector<ColorRGB>& pixels) {
        if (pixels.size() != width_ * height_) {
            throw std::invalid_argument(
                "Error: The number of pixels doesn't match the dimensions.");
        }

        pixels_ = pixels;
    }

    [[nodiscard]] std::vector<ColorRGB> get_pixels() const { return pixels_; }

private:
    std::vector<ColorRGB> pixels_;

    std::size_t width_  = 0;
    std::size_t height_ = 0;
};
