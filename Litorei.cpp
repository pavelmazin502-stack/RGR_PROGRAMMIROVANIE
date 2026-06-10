#include <iostream>
#include <string>
#include <vector>

#include "Litorei.h"
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
        {'б', 'щ'}, {'в', 'ш'}, {'г', 'ч'}, {'д', 'ц'}, {'ж', 'х'}, {'з', 'ф'},
        {'к', 'т'}, {'л', 'с'}, {'м', 'р'}, {'н', 'п'}, {'b', 'z'}, {'c', 'x'},
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
        {'щ', 'б'}, {'ш', 'в'}, {'ч', 'г'}, {'ц', 'д'},
        {'х', 'ж'}, {'ф', 'з'}, {'т', 'к'}, {'с', 'л'}, 
        {'р', 'м'}, {'п', 'н'}, {'z', 'b'}, {'x', 'c'}, 
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