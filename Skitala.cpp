// Шифр перестановки Скитала
#include <string>
#include <Skitala.h>

using namespace std;

string ScytaleCipher(string KeyStr, string message){
    // Проверяем, что ключ должен состоять только из цифр, то есть быть числом, а не текстом
    if (KeyStr.empty()){
        throw runtime_error("Произошла ошибка ввода, ключ не может быть пустым");
    }
    for (char c : KeyStr){
        if (!isdigit(c)){
            throw runtime_error("Произошла ошибка ввода, ключ должен быть целым числом");
        }
    }

    int keyInt = stoi(KeyStr); // Строка в целое число(количество строк)

    int lenght = ((message.size() + 1) / keyInt) + 1; // количество столбцов

    string result;

    for (size_t j = 0; j < lenght; ++j){
        for (size_t i = 0; i < lenght; ++i){
            if (i * lenght + j < message.size()){
                result += message[i * lenght + j];
            } else {
                result += ' ';
            }
        }
    }
    return result;
}

string deScytaleCipher(const string& KeyStr, const string& message){
     // Проверяем, что ключ должен состоять только из цифр, то есть быть числом, а не текстом
    if (KeyStr.empty()){
        throw runtime_error("Произошла ошибка ввода, ключ не может быть пустым");
    }
    for (char c : KeyStr){
        if (!isdigit(c)){
            throw runtime_error("Произошла ошибка ввода, ключ должен быть целым числом");
        }
    }
    int KeyInt = stoi(KeyStr);

    int rows = (message.size() + KeyInt - 1) / KeyInt;

    string result(message.size(), ' ');

    // Расшифровка по строкам
    size_t index = 0;
    for (int j = 0; j < KeyInt; j++) {     // Внешний цикл по исходным строкам
        for (int i = 0; i < rows; i++) {   // Внутренний цикл по исходным столбцам
            size_t pos = i * KeyInt + j;   // Вычисляем, куда должна вернуться буква
            if (pos < message.size() && index < message.size()) {
                result[pos] = message[index++];
            }
        }
    }
    // Удаление лишних пробелов
    size_t end = result.find_last_not_of(' ');
    if (end != string::npos) {
        result.resize(end + 1);
    } else {
        result.clear();
    }
    return result;
}