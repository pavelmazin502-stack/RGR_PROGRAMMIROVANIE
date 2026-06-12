#ifndef VIGENERE_H_INCLUDED
#define VIGENERE_H_INCLUDED

#include <string>
#include <vector>

std::string encryptVigenere(const std::string& plaintext, const std::string& key);
std::string decryptVigenere(const std::string& ciphertext, const std::string& key);

#endif