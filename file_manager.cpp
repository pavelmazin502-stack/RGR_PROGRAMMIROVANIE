#include "file_manager.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <iostream>

namespace fs = std::filesystem;

namespace file_manager {

    std::vector<uint8_t> read_file(const std::string& filePath) {
        if (!fs::exists(filePath)) {
            throw std::runtime_error("Ошибка: Файл не найден по указанному пути: " + filePath);
        }

        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка: Не удалось открыть файл для чтения.");
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            return buffer;
        } else {
            throw std::runtime_error("Ошибка: Сбой при чтении файла.");
        }
    }

    void write_file(const std::string& filePath, const std::vector<uint8_t>& data) {
        fs::path pathObj(filePath);
        fs::path dir = pathObj.parent_path();

        if (!dir.empty() && !fs::exists(dir)) {
            std::cout << "Директория не найдена. Создаю: " << dir.string() << std::endl;
            fs::create_directories(dir);
        }

        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка: Не удалось создать или открыть файл для записи.");
        }

        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        
        if (file.bad()) {
            throw std::runtime_error("Ошибка: Сбой записи данных на диск.");
        }
    }

}