// задание 6.2. самая длинная подстрока
// вариант 7

#include <iostream>
#include <string>
#include <sstream>
#include "hash_table.h"
#include "lab1/array.h"

// поиск самой длинной подстроки с различными символами
// алгоритм: скользящее окно + хеш-таблица
void findLongestSubstring(const std::string& s) {
    int n = s.length();
    
    if (n == 0) {
        std::cout << "Строка пуста. Максимальная длина: 0" << std::endl;
        return;
    }

    CharHashTable charPositions;
    
    int left = 0;           // левый указатель окна
    int maxLen = 0;         // максимальная длина подстроки
    int maxStart = 0;       // начальная позиция максимальной подстроки
    
    // двигаем правый указатель по строке
    for (int right = 0; right < n; ++right) {
        char currentChar = s[right];
        int charCode = static_cast<int>(currentChar);
        
        // если символ уже есть в текущем окне
        if (charPositions.contains(charCode)) {
            int prevPos = charPositions.search(charCode);
            
            // двигаем left вправо удаляя символы из таблицы
            while (left <= prevPos) {
                int leftCharCode = static_cast<int>(s[left]);
                charPositions.remove(leftCharCode);
                ++left;
            }
        }
        
        // добавляем текущий символ в таблицу
        charPositions.insert(charCode, right);
        
        // вычисляем длину текущего окна
        int currentLen = right - left + 1;
        
        // обновляем максимум
        if (currentLen > maxLen) {
            maxLen = currentLen;
            maxStart = left;
        }
    }
    
    // извлекаем максимальную подстроку
    std::string longestSubstring = s.substr(maxStart, maxLen);
    
    std::cout << "\n Результаты" << std::endl;
    std::cout << "Исходная строка: \"" << s << "\"" << std::endl;
    std::cout << "Длина строки: " << n << std::endl;
    std::cout << "Максимальная длина подстроки с различными символами: " << maxLen << std::endl;
    std::cout << "Самая длинная подстрока: \"" << longestSubstring << "\"" << std::endl;
}

int main() {
    std::cout << "Введите строку для поиска самой длинной подстроки:" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    std::string s;
    std::getline(std::cin, s);
    
    // сохранение строки в Strarray
    Strarray charsArray;
    for (char c : s) {
        charsArray.MPUSH_end(std::string(1, c));
    }
    
    std::cout << "\nСтрока сохранена в Strarray. Количество символов: " 
              << charsArray.MSIZE() << std::endl;
    
    // поиск самой длинной подстроки
    findLongestSubstring(s);
    
    return 0;
}