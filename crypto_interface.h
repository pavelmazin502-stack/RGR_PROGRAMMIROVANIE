#pragma once
#include <vector>
#include <string>
#include <cstdint>

// Макрос для унифицированного экспорта функций из DLL / .so
#if defined(_WIN32)
    #define EXPORT_API __declspec(dllexport)
#else
    #define EXPORT_API __attribute__((visibility("default")))
#endif

// Обязательно заворачиваем typedef в extern "C"!
// Это гарантирует, что main.cpp и библиотеки будут использовать одинаковое 
// соглашение о вызовах для передачи std::vector и std::string.
extern "C" {
    typedef std::vector<uint8_t> (*EncryptFunc)(const std::vector<uint8_t>& data, const std::string& key);
    typedef std::vector<uint8_t> (*DecryptFunc)(const std::vector<uint8_t>& data, const std::string& key);
    typedef std::string (*GetNameFunc)();
    typedef void (*GenerateKeysFunc)(std::string& publicKey, std::string& privateKey);
}