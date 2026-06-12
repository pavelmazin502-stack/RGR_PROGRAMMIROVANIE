#include <iostream>
#include <string>
#include <vector>

#include "Litorei.h"
#include "crypto_interface.h"
using namespace std;


// Распознавание русских букв
bool isRusUpper(unsigned char c) { // Распознавание заглавных русских букв
    return (c >= 192 && c <= 223);
}

bool isRusLower(unsigned char c) { // Распознавание строчных русских букв
    return (c >= 224 && c <= 255);
}

char toUpperRus(unsigned char symbol) {
    if (isRusLower(symbol)) return symbol - 32;
    return symbol;
}

char toLowerRus(unsigned char symbol) {
    if (isRusUpper(symbol)) return symbol + 32;
    return symbol;
}

string encryptLitoreya(const string& text) {
    vector<pair<char, char>> litoreya{
        {'\xe1', '\xf9'}, {'\xe2', '\xf8'}, {'\xe3', '\xf7'}, {'\xe4', '\xf6'}, {'\xe6', '\xf5'}, {'\xe7', '\xf4'},
        {'\xea', '\xf2'}, {'\xeb', '\xf1'}, {'\xec', '\xf0'}, {'\xed', '\xef'}, {'b', 'z'}, {'c', 'x'},
        {'d', 'w'}, {'f', 'v'}, {'g', 't'}, {'h', 's'}, {'j', 'r'}, {'k', 'q'},
        {'l', 'p'}, {'m', 'n'}
    };

    string encryptedText;
    for (char c : text) {
        char original = c;
        char lower = c;

        if (isRusUpper((unsigned char )c)) {
            lower = toLowerRus((unsigned char)c);
        }
        else if (isupper((unsigned char)c)) {
            lower = tolower((unsigned char)c);
        }

        bool replaced = false;
        for (const auto& pairSym : litoreya) {
            if (pairSym.first == lower) {
                if (isRusUpper((unsigned char)original)) {
                    encryptedText += toUpperRus((unsigned char)pairSym.second);
                }
                else if (isupper((unsigned char)original)) {
                    encryptedText += toupper((unsigned char)pairSym.second);
                }
                else {
                    encryptedText += pairSym.second;
                }
                replaced = true;
                break;
            }
            else if (pairSym.second == lower) {
                if (isRusUpper((unsigned char)original)) {
                    encryptedText += toUpperRus((unsigned char)pairSym.first);
                }
                else if (isupper((unsigned char)original)) {
                    encryptedText += toupper((unsigned char)pairSym.first);
                }
                else {
                    encryptedText += pairSym.first;
                }
                replaced = true;
                break;
            }
        }

        if (!replaced) {
            encryptedText += original;
        }
    }
    return encryptedText;
}

string decryptLitoreya(const string& text) {
    vector<pair<char, char>> litoreya{
        {'\xe1', '\xf9'}, {'\xe2', '\xf8'}, {'\xe3', '\xf7'}, {'\xe4', '\xf6'}, {'\xe6', '\xf5'}, {'\xe7', '\xf4'},
        {'\xea', '\xf2'}, {'\xeb', '\xf1'}, {'\xec', '\xf0'}, {'\xed', '\xef'}, {'b', 'z'}, {'c', 'x'},
        {'d', 'w'}, {'f', 'v'}, {'g', 't'}, {'h', 's'}, {'j', 'r'}, {'k', 'q'},
        {'l', 'p'}, {'m', 'n'},
        {'z', 'b'}, {'x', 'c'},
        {'w', 'd'}, {'v', 'f'}, {'t', 'g'}, {'s', 'h'},
        {'r', 'j'}, {'q', 'k'}, {'p', 'l'}, {'n', 'm'}};

    string decryptedText;
    for (char c : text) {
        char original = c;
        char lower = c;

        if (isRusUpper(c)) {
            lower = toLowerRus(c);
        } else if (isupper(c)) {
            lower = tolower(c);
        }

        bool replaced = false;
        for (auto pairSym : litoreya) {
            if (pairSym.first == lower) {
                if (isRusUpper((unsigned char)original)) {
                    decryptedText += toUpperRus((unsigned char )pairSym.second);
                } else if (isupper((unsigned char )original)) {
                    decryptedText += toupper((unsigned char )pairSym.second);
                } else {
                    decryptedText += pairSym.second;
                }
                replaced = true;
                break;
            } else if (pairSym.second == lower) {
                if (isRusUpper((unsigned char )original)) {
                    decryptedText += toUpperRus((unsigned char )pairSym.first);
                } else if (isupper((unsigned char )original)) {
                    decryptedText += toupper((unsigned char )pairSym.first);
                } else {
                    decryptedText += pairSym.first;
                }
                replaced = true;
                break;
            }
        }
        if (!replaced){
            decryptedText += original;
        }
    }
    return decryptedText;
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
