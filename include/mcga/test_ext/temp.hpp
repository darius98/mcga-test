#pragma once

#include <filesystem>
#include <fstream>

namespace mcga::test {

std::filesystem::path make_temp_file_name(std::string_view extension = "");

struct temp_file {
    std::filesystem::path file_path;
    std::ofstream writer;

    explicit temp_file(std::string_view extension = "");

    ~temp_file();
};

}  // namespace mcga::test
