#include "atbash.h"
#include "crypto_interface.h"
#include <locale>
#include <codecvt>

std::wstring apply_atbash(const std::wstring& input) {
    // Определяем алфавиты
    const std::wstring en_lower = L"abcdefghijklmnopqrstuvwxyz";
    const std::wstring en_upper = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::wstring ru_lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const std::wstring ru_upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    std::wstring result = L"";

    for (wchar_t c : input) {
        size_t pos;

        // Проверяем принадлежность символа к одному из алфавитов
        if ((pos = en_lower.find(c)) != std::wstring::npos) {
            result += en_lower[en_lower.length() - 1 - pos];
        }
        else if ((pos = en_upper.find(c)) != std::wstring::npos) {
            result += en_upper[en_upper.length() - 1 - pos];
        }
        else if ((pos = ru_lower.find(c)) != std::wstring::npos) {
            result += ru_lower[ru_lower.length() - 1 - pos];
        }
        else if ((pos = ru_upper.find(c)) != std::wstring::npos) {
            result += ru_upper[ru_upper.length() - 1 - pos];
        }
        else {
            // Если это пробел, цифра или знак препинания — оставляем без изменений
            result += c;
        }
    }
    return result;
}




extern "C" {
    EXPORT_API std::string get_algorithm_name() {
        return "Атбаш (бесключевой)";
    }

    EXPORT_API void generate_keys(std::string& publicKey, std::string& privateKey) {
        publicKey = "";
        privateKey = "";
    }

    EXPORT_API std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        std::string text(data.begin(), data.end());
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wtext = converter.from_bytes(text);

        std::wstring encrypted = apply_atbash(wtext);

        std::string result_str = converter.to_bytes(encrypted);
        return std::vector<uint8_t>(result_str.begin(), result_str.end());
    }

    EXPORT_API std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        // Атбаш симметричен: шифрование и расшифровка - это одна и та же операция
        return encrypt_data(data, key);
    }
}
