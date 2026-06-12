#include "atbash.h"
#include "crypto_interface.h"
#include <vector>

using namespace std;

// Разбор UTF-8 строки на символы
vector<string> splitUtf8_atbash(const string& str) {
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

const vector<string> en_lower = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
const vector<string> en_upper = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
const vector<string> ru_lower = {"а","б","в","г","д","е","ё","ж","з","и","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ъ","ы","ь","э","ю","я"};
const vector<string> ru_upper = {"А","Б","В","Г","Д","Е","Ё","Ж","З","И","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф","Х","Ц","Ч","Ш","Щ","Ъ","Ы","Ь","Э","Ю","Я"};

int getIndex(const vector<string>& arr, const string& val) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == val) return i;
    }
    return -1;
}

std::string apply_atbash(const std::string& input) {
    std::string result = "";
    vector<string> chars = splitUtf8_atbash(input);

    for (const string& c : chars) {
        int pos = -1;
        if ((pos = getIndex(en_lower, c)) != -1) {
            result += en_lower[en_lower.size() - 1 - pos];
        }
        else if ((pos = getIndex(en_upper, c)) != -1) {
            result += en_upper[en_upper.size() - 1 - pos];
        }
        else if ((pos = getIndex(ru_lower, c)) != -1) {
            result += ru_lower[ru_lower.size() - 1 - pos];
        }
        else if ((pos = getIndex(ru_upper, c)) != -1) {
            result += ru_upper[ru_upper.size() - 1 - pos];
        }
        else {
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
        std::string encrypted = apply_atbash(text);
        return std::vector<uint8_t>(encrypted.begin(), encrypted.end());
    }

    EXPORT_API std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        return encrypt_data(data, key);
    }
}
