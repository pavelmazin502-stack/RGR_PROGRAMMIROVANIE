#include "vigenere.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <clocale>
#include <stdexcept>

using namespace std;

// Русский алфавит (33 буквы с Ё)
const string RUS_UPPER = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
const string RUS_LOWER = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
const string LATIN_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string LATIN_LOWER = "abcdefghijklmnopqrstuvwxyz";

// Прототипы функций
int findIndex(const string& str, const string& ch);

// Проверка, является ли символ буквой (латиница или кириллица)
bool isLetter(const string& ch) {
    if (ch.size() == 1) {
        unsigned char c = ch[0];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    }
    // Проверка на русскую букву
    if (findIndex(RUS_UPPER, ch) >= 0 || findIndex(RUS_LOWER, ch) >= 0) return true;
    return false;
}

// Пойиск символа в строке
int findIndex(const string& str, const string& ch) {
    for (size_t i = 0; i < str.length(); ) {
        size_t len = 1;
        if ((str[i] & 0x80) != 0) {
            if ((str[i] & 0xE0) == 0xC0) len = 2;
            else if ((str[i] & 0xF0) == 0xE0) len = 3;
        }
        if (str.substr(i, len) == ch) return i / 2;
        i += len;
    }
    return -1;
}

// Преобразование индекса в UTF-8 символ
string indexToChar(const string& alphabet, int idx) {
    int count = 0;
    for (size_t i = 0; i < alphabet.length(); ) {
        size_t len = 1;
        if ((alphabet[i] & 0x80) != 0) {
            if ((alphabet[i] & 0xE0) == 0xC0) len = 2;
            else if ((alphabet[i] & 0xF0) == 0xE0) len = 3;
        }
        if (count == idx) return alphabet.substr(i, len);
        count++;
        i += len;
    }
    return "";
}

// Получение сдвига из символа ключа
int getShiftValue(const string& keyChar) {
    int idx = findIndex(LATIN_UPPER, keyChar);
    if (idx >= 0) return idx;
    idx = findIndex(LATIN_LOWER, keyChar);
    if (idx >= 0) return idx;
    if (keyChar.size() == 1 && isdigit(keyChar[0])) return keyChar[0] - '0';
    idx = findIndex(RUS_UPPER, keyChar);
    if (idx >= 0) return idx;
    idx = findIndex(RUS_LOWER, keyChar);
    if (idx >= 0) return idx;
    return 0;
}

// Разбиение UTF-8 строки на символы
vector<string> splitUtf8(const string& str) {
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

string encryptVigenere(const string& plaintext, const string& keyStr) {
    if (keyStr.empty()) throw runtime_error("Ключ не может быть пустым");
    
    vector<string> text = splitUtf8(plaintext);
    vector<string> key = splitUtf8(keyStr);
    
    vector<int> shifts;
    for (const string& k : key) {
        shifts.push_back(getShiftValue(k));
    }
    
    string result;
    int shiftPos = 0;
    int rusSize = 33;
    
    for (const string& c : text) {
        if (!isLetter(c)) {
            // Не буквы (пробелы, знаки) — просто добавляем, ключ не двигаем
            result += c;
            continue;
        }
        
        int shift = shifts[shiftPos % shifts.size()];
        
        int idx = findIndex(LATIN_UPPER, c);
        if (idx >= 0) {
            result += LATIN_UPPER.substr(idx, 1);
            shiftPos++;
            continue;
        }
        
        idx = findIndex(LATIN_LOWER, c);
        if (idx >= 0) {
            result += LATIN_LOWER.substr(idx, 1);
            shiftPos++;
            continue;
        }
        
        idx = findIndex(RUS_UPPER, c);
        if (idx >= 0) {
            int newIdx = (idx + shift) % rusSize;
            result += indexToChar(RUS_UPPER, newIdx);
            shiftPos++;
            continue;
        }
        
        idx = findIndex(RUS_LOWER, c);
        if (idx >= 0) {
            int newIdx = (idx + shift) % rusSize;
            result += indexToChar(RUS_LOWER, newIdx);
            shiftPos++;
            continue;
        }
        
        result += c;
    }
    
    return result;
}

string decryptVigenere(const string& ciphertext, const string& keyStr) {
    if (keyStr.empty()) throw runtime_error("Ключ не может быть пустым");
    
    vector<string> text = splitUtf8(ciphertext);
    vector<string> key = splitUtf8(keyStr);
    
    vector<int> shifts;
    for (const string& k : key) {
        shifts.push_back(getShiftValue(k));
    }
    
    string result;
    int shiftPos = 0;
    int rusSize = 33;
    
    for (const string& c : text) {
        if (!isLetter(c)) {
            result += c;
            continue;
        }
        
        int shift = shifts[shiftPos % shifts.size()];
        
        int idx = findIndex(LATIN_UPPER, c);
        if (idx >= 0) {
            result += LATIN_UPPER.substr(idx, 1);
            shiftPos++;
            continue;
        }
        
        idx = findIndex(LATIN_LOWER, c);
        if (idx >= 0) {
            result += LATIN_LOWER.substr(idx, 1);
            shiftPos++;
            continue;
        }
        
        idx = findIndex(RUS_UPPER, c);
        if (idx >= 0) {
            int newIdx = (idx - shift + rusSize) % rusSize;
            result += indexToChar(RUS_UPPER, newIdx);
            shiftPos++;
            continue;
        }
        
        idx = findIndex(RUS_LOWER, c);
        if (idx >= 0) {
            int newIdx = (idx - shift + rusSize) % rusSize;
            result += indexToChar(RUS_LOWER, newIdx);
            shiftPos++;
            continue;
        }
        
        result += c;
    }
    
    return result;
}