#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lab1/array.h" // Straarray - для парсинга аргументов и хранения операций
#include "lab1/singly_linked_list.h" // SinglyLinkedList - цепочки для разрешения коллизий

// константы и хэш-функция

const int HASH_SIZE = 1009;

// хеш функция для строки
// преобразует строку в индекс массива
int hashFunction(const std::string& key) {
    unsigned long hash = 5381; // начальное значение хеша
    for (char c : key) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return static_cast<int>(hash % HASH_SIZE);
}

// проверка наличия элемента в списке
bool contains(Node* head, const std::string& value) {
    Node* curr = head;
    while (curr != nullptr) {
        if (curr->data == value) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

struct Args {
    std::string filePath; // путь к файлу
    std::string query; // команды
};

// парсинг аргументов командной строки
 Args parseArgs(int argc, char* argv[]) {
    Args args;

    // сохранение всех аргументов
    Strarray argList;
    for (int i = 1; i < argc; ++i) {
        argList.MPUSH_end(std::string(argv[i]));
    }

    // поиск флагов --file и --query
    for (int i = 0; i < argList.MSIZE(); ++i) {
        std::string arg = argList.MGET_value(i);

        if (arg == "--file" && i + 1 < argList.MSIZE()) {
            args.filePath = argList.MGET_value(i + 1);
            ++i;
        } else if (arg == "--query" && i + 1 < argList.MSIZE()) {
            args.query = argList.MGET_value(i + 1);
            ++i;
        }
    }

    return args;
 }

 // разбиение строки запросов на отедльные операции
 // возвращает Strarray со списком операций
 Strarray splitOperations(const std::string& query) {
    Strarray operations;
    std::string current = "";

    for (char c : query) {
        if (c == ';') {
            if (!current.empty()) {
                operations.MPUSH_end(current);
                current = "";
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        operations.MPUSH_end(current);
    }
    return operations;
 }

 // класс - множество на основе хеш таблицы с цепочками
 // использует SinglyLinkedList для хранения элементов в каждой ячейке
 class HashSet {
    private:
        Node** buskets; // массив указателей на головы списков (цепочки коллизий)
    public:
        // конструктор: массив пустых списков
        HashSet() {
            buskets = new Node*[HASH_SIZE];
            for (int i = 0; i < HASH_SIZE; ++i) {
                buskets[i] = nullptr; // списки изначально пусты
            }
        }

        // деструктор: освобождение памяти всех списков
        ~HashSet() {
            for (int i = 0; i < HASH_SIZE; ++i) {
                Node* curr = buskets[i];
                while (curr != nullptr) {
                    Node* temp = curr;
                    curr = curr->next;
                    delete temp;
                }
            }
            delete[] buskets;
        }
        
        // SETADD
        void add(const std::string& value) {
            int index = hashFunction(value);

            if (!::contains(buskets[index], value)) {
                buskets[index] = FPUSHTAIL(buskets[index], value);
            }
        }

        // SETDEL
        void remove(const std::string& value) {
            int index = hashFunction(value);

            buskets[index] = FDELVALUE(buskets[index], value);
        }

        // SET_AT
        bool contains(const std::string& value) {
            int index = hashFunction(value);

            return ::contains(buskets[index], value);
        }

        void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ошибка: не удалось открыть файл для записи " << filename << std::endl;
            return;
        }
        
        // проходим по всем ячейкам хеш-таблицы
        for (int i = 0; i < HASH_SIZE; ++i) {
            Node* curr = buskets[i];
            while (curr != nullptr) {
                file << curr->data << "\n";  // записываем каждый элемент
                curr = curr->next;
            }
        }
        
        file.close();
        std::cout << "\nМножество сохранено в файл: " << filename << std::endl;
    }
};

void processOperation(const std::string& operation, HashSet& set) {
    std::istringstream iss(operation);
    std::string command;
    std::string value;

    iss >> command >> value;

    if (command == "SETADD") {
        bool wasPresent = set.contains(value);
        set.add(value);

        if (wasPresent) {
            std::cout << command << " " << value << " - уже в множестве" << std::endl;
        } else {
            std::cout << command << " " << value << " - добавлен" << std::endl;
        }
    } else if (command == "SETDEL") {
        bool wasPresent = set.contains(value);
        set.remove(value);

        if (wasPresent) {
            std::cout << command << " " << value << " - удален" << std::endl;
        } else {
            std::cout << command << value << " - не найден" << std::endl;
        }
    } else if (command == "SET_AT") {
        bool result = set.contains(value);
        std::cout << command << " " << value << " - " << (result ? "true" : "false") << std::endl;
    } else {
        std::cout << "Неизвестная команда: " << command << std::endl;
    }
}

int main(int argc, char* argv[]) {
    Args args = parseArgs(argc, argv);

    if (args.filePath.empty() || args.query.empty()) {
        std::cout << "Использование: ./task2 --file <имя_файла> --query <операции>" << std::endl;
        return 1; 
    }

    HashSet set;

    std::ifstream file(args.filePath);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << args.filePath << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            set.add(line);
        }
    }
    file.close();

    Strarray queryOperations = splitOperations(args.query);

    for (int i = 0; i < queryOperations.MSIZE(); ++i) {
        std::string op = queryOperations.MGET_value(i);
        processOperation(op, set);
    }

    set.saveToFile(args.filePath);
    
    return 0;
}

