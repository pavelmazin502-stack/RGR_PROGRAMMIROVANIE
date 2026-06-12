#include <iostream>
#include <string>
#include <vector>

#include "Litorei.h"
#include "crypto_interface.h"
using namespace std;

// Разбор UTF-8 строки на символы
vector<string> splitUtf8_litoreya(const string& str) {
    vector<string> chars;
    for (size_t i = 0; i < str.length(); ) {
        size_t len = 1;
        if ((str[i] & 0x80) != 0) {
            if ((str[i] & 0xE0) == 0xC0) len = 2;
            else if ((str[i] & 0xF0) == 0xE0) len = 3;
            else if ((str[i] & 0xF8) == 0xF0) len = 4;
        }
        chars.push_back(str.substr(i, len));
        i += len;
    }
    return chars;
}

const vector<pair<string, string>> litoreya_map = {
    {"б", "щ"}, {"в", "ш"}, {"г", "ч"}, {"д", "ц"}, {"ж", "х"}, {"з", "ф"},
    {"к", "т"}, {"л", "с"}, {"м", "р"}, {"н", "п"},
    {"Б", "Щ"}, {"В", "Ш"}, {"Г", "Ч"}, {"Д", "Ц"}, {"Ж", "Х"}, {"З", "Ф"},
    {"К", "Т"}, {"Л", "С"}, {"М", "Р"}, {"Н", "П"},
    {"b", "z"}, {"c", "x"}, {"d", "w"}, {"f", "v"}, {"g", "t"}, {"h", "s"},
    {"j", "r"}, {"k", "q"}, {"l", "p"}, {"m", "n"},
    {"B", "Z"}, {"C", "X"}, {"D", "W"}, {"F", "V"}, {"G", "T"}, {"H", "S"},
    {"J", "R"}, {"K", "Q"}, {"L", "P"}, {"M", "N"}
};

string encryptLitoreya(const string& text) {
    string encryptedText;
    vector<string> chars = splitUtf8_litoreya(text);
    for (const string& c : chars) {
        bool replaced = false;
        for (const auto& pairSym : litoreya_map) {
            if (pairSym.first == c) {
                encryptedText += pairSym.second;
                replaced = true;
                break;
            } else if (pairSym.second == c) {
                encryptedText += pairSym.first;
                replaced = true;
                break;
            }
        }
        if (!replaced) {
            encryptedText += c;
        }
    }
    return encryptedText;
}

string decryptLitoreya(const string& text) {
    return encryptLitoreya(text);
}

extern "C" {
    EXPORT_API std::string get_algorithm_name() {
        return "Литорея (бесключевой)";
    }

    EXPORT_API void generate_keys(std::string& publicKey, std::string& privateKey) {
        // Ключи не используются
        publicKey = "";
        privateKey = "";
    }

    EXPORT_API std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        std::string text(data.begin(), data.end());
        std::string encrypted = encryptLitoreya(text);
        return std::vector<uint8_t>(encrypted.begin(), encrypted.end());
    }

    EXPORT_API std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        std::string text(data.begin(), data.end());
        std::string decrypted = decryptLitoreya(text);
        return std::vector<uint8_t>(decrypted.begin(), decrypted.end());
    }
}
