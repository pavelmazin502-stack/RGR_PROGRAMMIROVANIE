#include "atbash.h"

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