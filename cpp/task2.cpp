
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstddef>

// Множество целых чисел от 0 до 10000
// Используется прямое адресование
class IntegerSet {
private:
    static constexpr int MIN_VALUE = 0;
    static constexpr int MAX_VALUE = 10000;

    std::array<bool, MAX_VALUE + 1> elements{};
    std::size_t elementCount = 0;

    bool isValid(int value) const {
        return value >= MIN_VALUE && value <= MAX_VALUE;
    }

public:
    bool add(int value) {
        if (!isValid(value) || elements[value]) {
            return false;
        }

        elements[value] = true;
        ++elementCount;
        return true;
    }

    bool remove(int value) {
        if (!isValid(value) || !elements[value]) {
            return false;
        }

        elements[value] = false;
        --elementCount;
        return true;
    }

    bool contains(int value) const {
        return isValid(value) && elements[value];
    }

    std::size_t size() const {
        return elementCount;
    }

    // Загружает элементы из файла
    bool loadFromFile(const std::string& fileName) {
        std::ifstream file(fileName);

        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл.\n";
            return false;
        }

        int value;

        while (file >> value) {
            if (!add(value)) {
                if (value < MIN_VALUE || value > MAX_VALUE) {
                    std::cerr << "Некорректный элемент: "
                              << value << '\n';
                }
            }
        }

        if (!file.eof()) {
            std::cerr << "Ошибка чтения файла.\n";
            return false;
        }

        return true;
    }

    // Сохраняет текущее множество в файл
    bool saveToFile(const std::string& fileName) const {
        std::ofstream file(fileName, std::ios::trunc);

        if (!file.is_open()) {
            std::cerr << "Не удалось сохранить файл.\n";
            return false;
        }

        for (int i = MIN_VALUE; i <= MAX_VALUE; ++i) {
            if (elements[i]) {
                file << i << '\n';
            }
        }

        return file.good();
    }
};

bool parseValue(const std::string& text, int& value) {
    std::istringstream parser(text);
    char extra;

    return (parser >> value) && !(parser >> extra);
}

// Выполнение команд множества
void processCommands(std::istream& input, IntegerSet& set) {
    std::string command;
    std::string rawValue;

    while (input >> command) {
        if (!(input >> rawValue)) {
            std::cerr << "Не указано значение для команды.\n";
            break;
        }

        int value = 0;

        if (!parseValue(rawValue, value) ||
            value < 0 || value > 10000) {
            std::cerr << "Значение должно быть от 0 до 10000.\n";
            continue;
        }

        if (command == "SETADD") {
            std::cout << (set.add(value) ? "OK" : "EXISTS") << '\n';
        }
        else if (command == "SETDEL") {
            std::cout << (set.remove(value) ? "OK" : "NOT FOUND") << '\n';
        }
        else if (command == "SET_AT") {
            std::cout << (set.contains(value) ? "true" : "false") << '\n';
        }
        else {
            std::cerr << "Неизвестная команда: "
                      << command << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    std::string fileName;
    std::string query;

    bool fileSpecified = false;
    bool querySpecified = false;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        std::string argument = argv[i];

        if (argument == "--file" && i + 1 < argc) {
            fileName = argv[++i];
            fileSpecified = true;
        }
        else if (argument == "--query" && i + 1 < argc) {
            query = argv[++i];
            querySpecified = true;
        }
        else {
            std::cerr << "Некорректный аргумент: "
                      << argument << '\n';
            return 1;
        }
    }

    IntegerSet set;

    // Загрузка существующих элементов
    if (fileSpecified) {
        std::ifstream check(fileName);

        if (!check.is_open()) {
            std::ofstream create(fileName);

            if (!create.is_open()) {
                std::cerr << "Не удалось создать файл.\n";
                return 1;
            }
        }
        else {
            check.close();

            if (!set.loadFromFile(fileName)) {
                return 1;
            }
        }
    }

    // Выполнение запроса
    if (querySpecified) {
        std::istringstream queryStream(query);
        processCommands(queryStream, set);
    }
    else {
        processCommands(std::cin, set);
    }

    // Сохранение обновлённого множества
    if (fileSpecified) {
        if (!set.saveToFile(fileName)) {
            return 1;
        }
    }

    return 0;
}
