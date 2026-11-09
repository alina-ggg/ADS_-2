// задание 6.1. вариант 1
// линейное апробирование и двойное хеширование

#include <iostream>
#include <string>
#include <sstream>
#include "hash_table.h"
#include "lab1/array.h"

int main() {
    std::cout << "Введите пары ключ:значение через пробел:" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    std::string inputLine;
    std::getline(std::cin, inputLine);
    
    if (inputLine.empty()) {
        std::cout << "Ошибка: пустой ввод!" << std::endl;
        return 1;
    }
    
    Strarray keysStr, valuesStr;
    std::istringstream iss(inputLine);
    std::string token;
    
    while (iss >> token) {
        size_t pos = token.find(':');
        if (pos != std::string::npos) {
            keysStr.MPUSH_end(token.substr(0, pos));
            valuesStr.MPUSH_end(token.substr(pos + 1));
        } else {
            std::cout << "Ошибка: неверный формат '" << token << "'. Используйте ключ:значение" << std::endl;
            return 1;
        }
    }
    
    if (keysStr.MSIZE() == 0) {
        std::cout << "Ошибка: не введено ни одной пары!" << std::endl;
        return 1;
    }
    
    std::cout << "\nВведено " << keysStr.MSIZE() << " пар:" << std::endl;
    for (int i = 0; i < keysStr.MSIZE(); ++i) {
        std::cout << "  [" << keysStr.MGET_value(i) << " : " << valuesStr.MGET_value(i) << "]" << std::endl;
    }
    
    HashTableLinear tableLinear;
    HashTableDouble tableDouble;
    
    std::cout << "\n Вставка значений" << std::endl;
    for (int i = 0; i < keysStr.MSIZE(); ++i) {
        int key = std::stoi(keysStr.MGET_value(i));
        int value = std::stoi(valuesStr.MGET_value(i));
        
        int idx1_before = tableLinear.search(key);
        
        bool ok1 = tableLinear.insert(key, value);
        bool ok2 = tableDouble.insert(key, value);
        
        std::cout << "Вставка [" << key << ":" << value << "]: ";
        if (ok1 && ok2) {
            if (idx1_before != -1) {
                std::cout << "ключ обновлен (перезаписан) в обеих таблицах" << std::endl;
            } else {
                std::cout << "успешно в обе таблицы" << std::endl;
            }
        } else {
            std::cout << "ошибка: таблицы заполнены!" << std::endl;
        }
    }
    
    tableLinear.print();
    tableDouble.print();
    
    std::cout << "\n Поиск элементов" << std::endl;
    for (int i = 0; i < keysStr.MSIZE(); ++i) {
        int key = std::stoi(keysStr.MGET_value(i));
        int idx1 = tableLinear.search(key);
        int idx2 = tableDouble.search(key);
        
        std::string val1 = (idx1 != -1) ? tableLinear.getValue(key) : "не найдено";
        std::string val2 = (idx2 != -1) ? tableDouble.getValue(key) : "не найдено";
        
        std::cout << "Поиск ключа " << key << ": " 
                  << "линейная[ячейка " << idx1 << ", значение " << val1 << "], "
                  << "двойная[ячейка " << idx2 << ", значение " << val2 << "]" << std::endl;
    }
   
    std::cout << "\n Удаление элемента" << std::endl;
    std::cout << "Введите ключ для удаления (или 0 для пропуска): ";
    std::cout.flush();
    
    int toDelete;
    std::cin >> toDelete;
    
    if (toDelete != 0) {
        std::cout << "\nУдаляем ключ " << toDelete << "..." << std::endl;
        
        bool removed1 = tableLinear.remove(toDelete);
        bool removed2 = tableDouble.remove(toDelete);
        
        if (removed1 && removed2) {
            std::cout << "Ключ " << toDelete << " удалён из обеих таблиц." << std::endl;
        } else if (removed1) {
            std::cout << "Ключ " << toDelete << " удалён из линейной, не найден в двойной." << std::endl;
        } else if (removed2) {
            std::cout << "Ключ " << toDelete << " удалён из двойной, не найден в линейной." << std::endl;
        } else {
            std::cout << "Ключ " << toDelete << " не найден ни в одной таблице!" << std::endl;
        }
        
        std::cout << "\nПосле удаления:" << std::endl;
        tableLinear.print();
        tableDouble.print();
    } else {
        std::cout << "Удаление пропущено." << std::endl;
    }
    
    std::cout << "\n Сравнение методов" << std::endl;
    std::cout << "Линейное апробирование:" << std::endl;
    std::cout << "  Формула: (h(k) + i) % m" << std::endl;
    std::cout << "  Достоинства: простота реализации, хороший кеш" << std::endl;
    std::cout << "  Недостатки: кластеризация (группировка занятых ячеек)" << std::endl;
    std::cout << std::endl;
    std::cout << "Двойное хеширование:" << std::endl;
    std::cout << "  Формула: (h1(k) + i * h2(k)) % m" << std::endl;
    std::cout << "  Достоинства: меньше кластеризации, равномерное распределение" << std::endl;
    std::cout << "  Недостатки: сложнее, требует вторую хеш-функцию" << std::endl;
    
    return 0;
}