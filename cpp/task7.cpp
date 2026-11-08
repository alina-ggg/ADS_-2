// 1 вариант — LRU-кэш
#include <iostream>
#include <string>
#include <sstream>
#include "lab1/array.h" // Strarray — хранение пар "ключ:значение"
#include "lab1/singly_linked_list.h" // SinglyLinkedList — порядок использования ключей

class LRUCache {
private:
    int capacity; // емкость кэша
    Strarray cache; // пары ключ:значение
    Node* orderList; // список порядка использования
    int currentSize; // текущий размер кэша
    
    // найти индекс пары ключ:значение в Strarray по ключу
    // возвращает индекс или -1, если не найден
    int findKeyIndex(const std::string& key) {
        std::string prefix = key + ":";
        for (int i = 0; i < cache.MSIZE(); ++i) {
            std::string entry = cache.MGET_value(i);
            // проверяем, начинается ли запись с key:
            if (entry.length() >= prefix.length() && 
                entry.substr(0, prefix.length()) == prefix) {
                return i;
            }
        }
        return -1;
    }
    
    // извлечь значение из пары ключ:значение
    std::string getValue(const std::string& entry) {
        size_t pos = entry.find(':');
        if (pos != std::string::npos) {
            return entry.substr(pos + 1);
        }
        return "";
    }
    
    // удалить ключ из списка порядка использования
    void removeFromOrderList(const std::string& key) {
        orderList = FDELVALUE(orderList, key);
    }
    
    // добавить ключ в начало списка
    void addToFrontOfOrderList(const std::string& key) {
        orderList = FPUSHHEAD(orderList, key);
    }
    
public:
    // конструктор: инициализация кэша заданной емкости
    LRUCache(int cap) : capacity(cap), cache(), orderList(nullptr), currentSize(0) {
        // Strarray инициализируется конструктором по умолчанию
    }
    
    // деструктор: освобождение памяти списка
    ~LRUCache() {
        while (orderList != nullptr) {
            orderList = FDELHEAD(orderList);
        }
    }
    
    // SET x y: установить значение ключа x равным y
    void set(const std::string& key, const std::string& value) {
        std::string entry = key + ":" + value;
        int idx = findKeyIndex(key);
        
        if (idx != -1) {
            // ключ уже есть — обновляем значение
            cache.MREPLACE(idx, entry);
            
            // перемещаем ключ в начало списка
            removeFromOrderList(key);
            addToFrontOfOrderList(key);
        } else {
            // ключа нет - добавляем новый
            if (currentSize == capacity) {
                // кэш полон - удаляем наименее недавно использованный
                Node* curr = orderList;
                Node* prev = nullptr;
                Node* last = nullptr;
                Node* lastPrev = nullptr;
                
                // находим последний узел и предпоследний
                while (curr != nullptr) {
                    lastPrev = prev;
                    last = curr;
                    prev = curr;
                    curr = curr->next;
                }
                
                if (last != nullptr) {
                    std::string lruKey = last->data;
                    
                    // удаляем последний узел из списка
                    if (lastPrev == nullptr) {
                        orderList = FDELHEAD(orderList);
                    } else {
                        lastPrev->next = nullptr;
                        delete last;
                    }
            
                    int lruIdx = findKeyIndex(lruKey);
                    if (lruIdx != -1) {
                        cache.MDEL(lruIdx);
                    }
                    
                    --currentSize;
                }
            }
            
            // добавляем новую пару
            cache.MPUSH_end(entry);
            addToFrontOfOrderList(key);
            ++currentSize;
        }
    }
    
    // GET x: вернуть значение ключа x или -1
    std::string get(const std::string& key) {
        int idx = findKeyIndex(key);
        
        if (idx == -1) {
            return "-1";  // ключ не найден
        }
        
        // ключ найден - перемещаем в начало списка
        std::string entry = cache.MGET_value(idx);
        std::string value = getValue(entry);
        
        removeFromOrderList(key);
        addToFrontOfOrderList(key);
        
        return value;
    }
    
    // вывод текущего состояния кэша
    void print() const {
        std::cout << "Кэш (размер " << currentSize << "/" << capacity << "):" << std::endl;
        for (int i = 0; i < cache.MSIZE(); ++i) {
            std::cout << "  " << cache.MGET_value(i) << std::endl;
        }
        std::cout << "Порядок использования: ";
        ::print(orderList);
    }
};

int main() {
    std::cout << "Введите емкость кэша (cap):" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    int cap;
    std::cin >> cap;
    
    if (cap <= 0) {
        std::cout << "Ошибка: емкость должна быть положительной!" << std::endl;
        return 1;
    }
    
    std::cout << "\nВведите количество запросов (Q):" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    int Q;
    std::cin >> Q;
    
    if (Q <= 0) {
        std::cout << "Ошибка: количество запросов должно быть положительным!" << std::endl;
        return 1;
    }
    
    std::cout << "\nВведите запросы (SET x y или GET x) через пробел:" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    std::string inputLine;
    std::cin.ignore(); 
    std::getline(std::cin, inputLine);

    Strarray tokens;
    std::istringstream iss(inputLine);
    std::string token;
    
    while (iss >> token) {
        tokens.MPUSH_end(token);
    }
   
    if (tokens.MSIZE() == 0) {
        std::cout << "Ошибка: не введено ни одного запроса!" << std::endl;
        return 1;
    }
    
    LRUCache cache(cap);
    
    std::cout << "\n Результаты" << std::endl;
    
    int i = 0;
    while (i < tokens.MSIZE()) {
        std::string command = tokens.MGET_value(i);
        
        if (command == "SET" && i + 2 < tokens.MSIZE()) {
            std::string key = tokens.MGET_value(i + 1);
            std::string value = tokens.MGET_value(i + 2);
            
            cache.set(key, value);
            std::cout << "SET " << key << " " << value << std::endl;
            i += 3;
        } else if (command == "GET" && i + 1 < tokens.MSIZE()) {
            std::string key = tokens.MGET_value(i + 1);
            std::string result = cache.get(key);
            
            std::cout << "GET " << key << " -> " << result << std::endl;
            i += 2;
        } else {
            std::cout << "Неизвестная команда: " << command << std::endl;
            ++i;
        }
    }
    
    std::cout << "\n Финальное состояние кэша" << std::endl;
    cache.print();
    
    return 0;
}