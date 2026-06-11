#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace file_manager {
    std::vector<uint8_t> read_file(const std::string& filePath);
    void write_file(const std::string& filePath, const std::vector<uint8_t>& data);
}