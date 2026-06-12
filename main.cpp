#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include <dlfcn.h>
#include "crypto_interface.h"
#include "file_manager.h"

namespace fs = std::filesystem;

struct Plugin {
    void* handle = nullptr;
    std::string filename;
    std::string name;
    EncryptFunc encrypt = nullptr;
    DecryptFunc decrypt = nullptr;
    GetNameFunc get_name = nullptr;
    GenerateKeysFunc generate_keys = nullptr;

    bool load(const std::string& path) {
        handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Ошибка загрузки плагина " << path << ": " << dlerror() << std::endl;
            return false;
        }

        get_name = (GetNameFunc)dlsym(handle, "get_algorithm_name");
        generate_keys = (GenerateKeysFunc)dlsym(handle, "generate_keys");
        encrypt = (EncryptFunc)dlsym(handle, "encrypt_data");
        decrypt = (DecryptFunc)dlsym(handle, "decrypt_data");

        if (!get_name || !generate_keys || !encrypt || !decrypt) {
            std::cerr << "Ошибка: Плагин " << path << " не реализует требуемый API." << std::endl;
            dlclose(handle);
            handle = nullptr;
            return false;
        }

        filename = fs::path(path).filename().string();
        name = get_name();
        return true;
    }

    void unload() {
        if (handle) {
            dlclose(handle);
            handle = nullptr;
        }
    }
};

std::vector<Plugin> load_plugins(const std::string& directory) {
    std::vector<Plugin> plugins;
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Директория плагинов '" << directory << "' не найдена." << std::endl;
        return plugins;
    }

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".so") {
            Plugin p;
            if (p.load(entry.path().string())) {
                plugins.push_back(p);
            }
        }
    }
    return plugins;
}

void print_menu(const Plugin* current_plugin) {
    std::cout << "\n=== Менеджер Шифрования ===" << std::endl;
    if (current_plugin) {
        std::cout << "Текущий алгоритм: " << current_plugin->name << std::endl;
    } else {
        std::cout << "Текущий алгоритм: Не выбран" << std::endl;
    }
    std::cout << "1. Выбрать алгоритм" << std::endl;
    std::cout << "2. Сгенерировать ключи (для текущего алгоритма)" << std::endl;
    std::cout << "3. Зашифровать текст" << std::endl;
    std::cout << "4. Расшифровать текст" << std::endl;
    std::cout << "5. Зашифровать файл" << std::endl;
    std::cout << "6. Расшифровать файл" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Ваш выбор: ";
}

int main() {
    // Получаем путь к директории плагинов относительно текущей рабочей директории
    std::string plugins_dir = "./plagins";
    std::vector<Plugin> plugins = load_plugins(plugins_dir);

    if (plugins.empty()) {
        std::cout << "Плагины не найдены. Создайте папку plugins/ и поместите туда .so файлы." << std::endl;
    } else {
        std::cout << "Успешно загружено " << plugins.size() << " плагинов." << std::endl;
    }

    Plugin* current_plugin = nullptr;
    int choice = -1;

    while (choice != 0) {
        print_menu(current_plugin);
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (choice) {
            case 1: {
                if (plugins.empty()) {
                    std::cout << "Нет доступных алгоритмов." << std::endl;
                    break;
                }
                std::cout << "\nДоступные алгоритмы:" << std::endl;
                for (size_t i = 0; i < plugins.size(); ++i) {
                    std::cout << i + 1 << ". " << plugins[i].name << " (" << plugins[i].filename << ")" << std::endl;
                }
                std::cout << "Выберите номер: ";
                size_t p_choice;
                std::cin >> p_choice;
                if (p_choice > 0 && p_choice <= plugins.size()) {
                    current_plugin = &plugins[p_choice - 1];
                    std::cout << "Выбран алгоритм: " << current_plugin->name << std::endl;
                } else {
                    std::cout << "Неверный выбор." << std::endl;
                }
                break;
            }
            case 2: {
                if (!current_plugin) {
                    std::cout << "Сначала выберите алгоритм." << std::endl;
                    break;
                }
                std::string pub, priv;
                current_plugin->generate_keys(pub, priv);
                if (pub.empty() && priv.empty()) {
                    std::cout << "Этот алгоритм не использует ключи или генерация не требуется." << std::endl;
                } else {
                    std::cout << "Публичный ключ (или ключ шифрования): " << pub << std::endl;
                    std::cout << "Приватный ключ (или ключ дешифрования): " << priv << std::endl;
                }
                break;
            }
            case 3:
            case 4: {
                if (!current_plugin) {
                    std::cout << "Сначала выберите алгоритм." << std::endl;
                    break;
                }
                std::cout << "Введите текст: ";
                std::string text;
                std::getline(std::cin, text);
                
                std::cout << "Введите ключ (если алгоритм бесключевой, нажмите Enter): ";
                std::string key;
                std::getline(std::cin, key);

                std::vector<uint8_t> data(text.begin(), text.end());
                std::vector<uint8_t> result;

                try {
                    if (choice == 3) {
                        result = current_plugin->encrypt(data, key);
                        std::cout << "Зашифрованный текст (hex): ";
                        for (uint8_t b : result) {
                            printf("%02X", b);
                        }
                        std::cout << "\nЗашифрованный текст (str): " << std::string(result.begin(), result.end()) << std::endl;
                    } else {
                        result = current_plugin->decrypt(data, key);
                        std::cout << "Расшифрованный текст: " << std::string(result.begin(), result.end()) << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка в процессе работы алгоритма: " << e.what() << std::endl;
                }
                break;
            }
            case 5:
            case 6: {
                if (!current_plugin) {
                    std::cout << "Сначала выберите алгоритм." << std::endl;
                    break;
                }
                std::cout << "Введите путь к исходному файлу: ";
                std::string in_path;
                std::getline(std::cin, in_path);
                
                std::cout << "Введите путь для сохранения: ";
                std::string out_path;
                std::getline(std::cin, out_path);

                std::cout << "Введите ключ (если требуется): ";
                std::string key;
                std::getline(std::cin, key);

                try {
                    std::vector<uint8_t> data = file_manager::read_file(in_path);
                    std::vector<uint8_t> result;
                    
                    if (choice == 5) {
                        result = current_plugin->encrypt(data, key);
                        std::cout << "Файл успешно зашифрован." << std::endl;
                    } else {
                        result = current_plugin->decrypt(data, key);
                        std::cout << "Файл успешно расшифрован." << std::endl;
                    }

                    file_manager::write_file(out_path, result);
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка работы с файлом: " << e.what() << std::endl;
                }
                break;
            }
            case 0:
                std::cout << "Выход..." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор." << std::endl;
        }
    }

    for (auto& p : plugins) {
        p.unload();
    }

    return 0;
}