#ifndef LITOREYA_H_INCLUDED
#define LITOREYA_H_INCLUDED
#include <unordered_map>
#include <vector>
bool isRusUpper(unsigned char c);
char toUpperRus(unsigned char symbol);
char toLowerRus(unsigned char symbol);
std::string encryptLitoreya(const std::string& text);
std::string decryptLitoreya(const std::string& text);
#endif // LITOREYA_H_INCLUDED