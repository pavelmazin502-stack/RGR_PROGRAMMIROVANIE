#include "crypto_interface.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <random> 

namespace rsa_internal {
    
    uint64_t gcd(uint64_t a, uint64_t b) {
        while (b != 0) {
            uint64_t temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
        if (mod == 0) throw std::invalid_argument("Modulo cannot be zero");
        uint64_t res = 1;
        base = base % mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (uint64_t)(((unsigned __int128)res * base) % mod);
            exp = exp >> 1;
            base = (uint64_t)(((unsigned __int128)base * base) % mod);
        }
        return res;
    }

    uint64_t modInverse(uint64_t a, uint64_t m) {
        int64_t m0 = m, t, q;
        int64_t x0 = 0, x1 = 1;
        int64_t a_signed = a;
        int64_t m_signed = m;
        if (m_signed == 1) return 0;
        while (a_signed > 1) {
            q = a_signed / m_signed;
            t = m_signed;
            m_signed = a_signed % m_signed, a_signed = t;
            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }
        if (x1 < 0) x1 += m0;
        return x1;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i = i + 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

     int generatePrime(int min, int max) {
        static std::random_device rd;  
        static std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> distr(min, max); 

        int p = distr(gen); 
        while (!isPrime(p)) {
            p++;
            if (p > max) p = min;
        }
        return p;
    }

    void parseKey(const std::string& keyStr, uint64_t& keyVal, uint64_t& n) {
        size_t colonPos = keyStr.find(':');
        if (colonPos == std::string::npos) {
            throw std::invalid_argument("Неверный формат ключа RSA.");
        }
        keyVal = std::stoull(keyStr.substr(0, colonPos));
        n = std::stoull(keyStr.substr(colonPos + 1));
    }
}

extern "C" {

    EXPORT_API std::string get_algorithm_name() {
        return "RSA Asymmetric Cipher (True Random)";
    }

    EXPORT_API void generate_keys(std::string& publicKey, std::string& privateKey) {

        uint64_t p = rsa_internal::generatePrime(500, 2000);
        uint64_t q = rsa_internal::generatePrime(500, 2000);
        while (p == q) q = rsa_internal::generatePrime(500, 2000);

        uint64_t n = p * q;
        uint64_t phi = (p - 1) * (q - 1);

        uint64_t e = 3;
        while (rsa_internal::gcd(e, phi) != 1) {
            e++;
        }

        uint64_t d = rsa_internal::modInverse(e, phi);

        publicKey = std::to_string(e) + ":" + std::to_string(n);
        privateKey = std::to_string(d) + ":" + std::to_string(n);
    }

    EXPORT_API std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        uint64_t e, n;
        rsa_internal::parseKey(key, e, n);
        std::vector<uint8_t> result;
        result.reserve(data.size() * 4); 

        for (uint8_t m : data) {
            uint64_t c = rsa_internal::modPow(m, e, n);
            result.push_back((c >> 24) & 0xFF);
            result.push_back((c >> 16) & 0xFF);
            result.push_back((c >> 8) & 0xFF);
            result.push_back(c & 0xFF);
        }
        return result;
    }

    EXPORT_API std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& data, const std::string& key) {
        uint64_t d, n;
        rsa_internal::parseKey(key, d, n);
        
        if (data.size() % 4 != 0) {
            throw std::invalid_argument("Неверный размер зашифрованных данных. Он должен быть кратен 4.");
        }
        
        std::vector<uint8_t> result;
        result.reserve(data.size() / 4);

        for (size_t i = 0; i < data.size(); i += 4) {
            uint64_t c = (uint64_t(data[i]) << 24) |
                         (uint64_t(data[i+1]) << 16) |
                         (uint64_t(data[i+2]) << 8) |
                         uint64_t(data[i+3]);
            uint64_t m = rsa_internal::modPow(c, d, n);
            result.push_back(static_cast<uint8_t>(m));
        }
        return result;
    }
}