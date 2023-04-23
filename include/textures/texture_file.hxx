#pragma once

#include "abstract_file.hxx"
#include "color.hxx"

#include <vector>

class TextureFile : public File {
public:
    TextureFile(const std::filesystem::path & file_path) : File(file_path) {
        set_path(file_path);
    }

    void load() override {

    }

    void save(const std::filesystem::path & file_path) override {
        std::ofstream out_file(file_path, std::ios::binary);
    }


private:
    std::vector<Color> pixels_;
};
