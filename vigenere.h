#ifndef VIGENERE_H_INCLUDED
#define VIGENERE_H_INCLUDED

#include <string>
#include <vector>

using namespace std;

string encryptVigenere(const string& plaintext, const string& key);
string decryptVigenere(const string& ciphertext, const string& key);

#endif