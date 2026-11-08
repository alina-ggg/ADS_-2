// задание 6.1. вариант 1
// линейное апробирование и двойное хеширование

#include <iostream>
#include <string>
#include <sstream>
#include "hash_table.h"
#include "lab1/array.h"

int main() {
    // ввод значений
    std::cout << "Введите значения для вставки в хеш-таблицу через пробел:" << std::endl;
    std::cout.flush();
    
    std::string inputLine;
    std::getline(std::cin, inputLine);
    
    if (inputLine.empty()) {
        std::cout << "Ошибка: пустой ввод!" << std::endl;
        return 1;
    }
    
    // разбираем строку на числа
    Strarray valuesStr;
    std::istringstream iss(inputLine);
    std::string token;
    
    while (iss >> token) {
        valuesStr.MPUSH_end(token);
    }
    
    if (valuesStr.MSIZE() == 0) {
        std::cout << "Ошибка: не введено ни одного значения!" << std::endl;
        return 1;
    }
    
    // сыводим введённые значения
    std::cout << "\nВведено " << valuesStr.MSIZE() << " значений: ";
    for (int i = 0; i < valuesStr.MSIZE(); ++i) {
        std::cout << valuesStr.MGET_value(i);
        if (i < valuesStr.MSIZE() - 1) std::cout << " ";
    }
    std::cout << std::endl;
    
    // создание двух хеш-таблиц
    HashTableLinear tableLinear;
    HashTableDouble tableDouble;
    
    // вставка значений в обе таблицы
    std::cout << "\nВставка значений" << std::endl;
    for (int i = 0; i < valuesStr.MSIZE(); ++i) {
        int num = std::stoi(valuesStr.MGET_value(i));
        
        bool ok1 = tableLinear.insert(num);
        bool ok2 = tableDouble.insert(num);
        
        std::cout << "Вставка " << num << ": ";
        if (ok1 && ok2) {
            std::cout << "успешно в обе таблицы" << std::endl;
        } else if (ok1) {
            std::cout << "успешно в линейную, двойная заполнена" << std::endl;
        } else if (ok2) {
            std::cout << "успешно в двойную, линейная заполнена" << std::endl;
        } else {
            std::cout << "обе таблицы заполнены!" << std::endl;
        }
    }
    
    // вывод таблиц
    tableLinear.print();
    tableDouble.print();
    
    // демонстрация поиска
    std::cout << "\n Поиск элементов " << std::endl;
    for (int i = 0; i < valuesStr.MSIZE(); ++i) {
        int num = std::stoi(valuesStr.MGET_value(i));
        int idx1 = tableLinear.search(num);
        int idx2 = tableDouble.search(num);
        std::cout << "Поиск " << num << ": линейная[ячейка " << idx1 
                  << "], двойная[ячейка " << idx2 << "]" << std::endl;
    }
    
    // демонстрация удаления
    std::cout << "\n Удаление элемента" << std::endl;
    std::cout << "Введите число для удаления (или 0 для пропуска): ";
    std::cout.flush();
    
    int toDelete;
    std::cin >> toDelete;
    
    if (toDelete != 0) {
        std::cout << "\nУдаляем элемент " << toDelete << "..." << std::endl;
        
        bool removed1 = tableLinear.remove(toDelete);
        bool removed2 = tableDouble.remove(toDelete);
        
        if (removed1 && removed2) {
            std::cout << "Элемент " << toDelete << " удалён из обеих таблиц." << std::endl;
        } else if (removed1) {
            std::cout << "Элемент " << toDelete << " удалён из линейной, не найден в двойной." << std::endl;
        } else if (removed2) {
            std::cout << "Элемент " << toDelete << " удалён из двойной, не найден в линейной." << std::endl;
        } else {
            std::cout << "Элемент " << toDelete << " не найден ни в одной таблице!" << std::endl;
        }
        
        std::cout << "\nПосле удаления:" << std::endl;
        tableLinear.print();
        tableDouble.print();
    } else {
        std::cout << "Удаление пропущено." << std::endl;
    }
    
    // теоретическое сравнение методов
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