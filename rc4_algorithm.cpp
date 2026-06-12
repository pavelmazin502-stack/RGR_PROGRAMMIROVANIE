#include "crypto_interface.h"
#include <string>
#include <vector>
#include <random> 

namespace rc4_internal {

    void process_rc4(const std::vector<uint8_t>& input, const std::string& key, std::vector<uint8_t>& output) {
        if (key.empty()) {
            throw std::invalid_argument("Ключ не может быть пустым для RC4");
        }

        std::vector<uint8_t> S(256);
        for (int i = 0; i < 256; i++) {
            S[i] = static_cast<uint8_t>(i);
        }

        int j = 0;
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + key[i % key.length()]) % 256;
            std::swap(S[i], S[j]);
        }

        int i = 0;
        j = 0;
        output.resize(input.size());
        for (size_t n = 0; n < input.size(); n++) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            std::swap(S[i], S[j]);
            
            uint8_t K = S[(S[i] + S[j]) % 256]; 
            output[n] = input[n] ^ K;          
        }
    }
}

extern "C" {

    EXPORT_API std::string get_algorithm_name() {
        return "RC4 Symmetric Cipher (True Random)";
    }

    EXPORT_API void generate_keys(std::string& publicKey, std::string& privateKey) {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::uniform_int_distribution<> distr(0, sizeof(charset) - 2);

        std::string generated_key = "";
        int keyLength = 16; 
        for (int i = 0; i < keyLength; i++) {
            generated_key += charset[distr(gen)];
        }

        publicKey = generated_key;
        privateKey = generated_key;
    }

    EXPORT_API std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        std::vector<uint8_t> result;
        rc4_internal::process_rc4(data, key, result);
        return result;
    }

    EXPORT_API std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        std::vector<uint8_t> result;
        rc4_internal::process_rc4(data, key, result);
        return result;
    }
}