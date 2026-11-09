// 1 вариант
#include <iostream>
#include <string>
#include <sstream>
#include "lab1/array.h" // Strarray - хранение множества, флагов, результатов
#include "lab1/stack.h" // Stack - backtracking (перебор с возвратом)
#include "hash_set.h"

// преобразование строки в целое число
int toInt(const std::string& s) {
    return std::stoi(s);
}

// преобразование целого числа в строку
std::string toString(int n) {
    return std::to_string(n);
}

int sumHashSet(const HashSet& set) {
    int sum = 0;
    Strarray elements = set.getElements();
    for (int i = 0; i < elements.MSIZE(); ++i) {
        sum += toInt(elements.MGET_value(i));
    }
    return sum;
}

bool backtrack(
    const Strarray& inputOrder, // исходное множество чисел
    HashSet& used, // флаги использования
    int targetSum, // требуемая сумма подмножества
    Stack& currentSubset, // текущее формируемое подмножество
    int currentSum, // текущая сумма в формируемом подмножестве
    Strarray& result, // результат
    int subsetsFound, // количество уже найденных подмножеств
    int totalSubsets // общее количество подмножеств
) {
    // случай: все подмножества найдены
    if (subsetsFound == totalSubsets) {
        return true;
    }

   if (currentSum == targetSum) {
        Strarray poppedElements;
        while (!currentSubset.isEmpty()) {
            poppedElements.MPUSH_end(currentSubset.pop());
        }
    
        std::string subsetStr = "";
        for (int i = poppedElements.MSIZE() - 1; i >= 0; --i) {
            if (i < poppedElements.MSIZE() - 1) {
                subsetStr += ", ";
            }
            subsetStr += poppedElements.MGET_value(i); 
        }
    
        result.MPUSH_end("{" + subsetStr + "}");
    
    
        if (backtrack(inputOrder, used, targetSum, currentSubset, 0, result, subsetsFound + 1, totalSubsets)) {
            return true;
        }

        result.MDEL(result.MSIZE() - 1);

        for (int i = poppedElements.MSIZE() - 1; i >= 0; --i) {
            currentSubset.push(poppedElements.MGET_value(i));
        }

        for (int i = 0; i < poppedElements.MSIZE(); ++i) {
            used.remove(poppedElements.MGET_value(i));
        }
        
        return false;
    }

    for (int i = 0; i < inputOrder.MSIZE(); ++i) {
        std::string elem = inputOrder.MGET_value(i);
        
        if (used.contains(elem)) continue;
        
        int elemValue = toInt(elem);
        if (currentSum + elemValue > targetSum) continue;
        
        used.add(elem);
        currentSubset.push(elem);
        
        if (backtrack(inputOrder, used, targetSum, currentSubset, currentSum + elemValue, result, subsetsFound, totalSubsets)) {
            return true;
        }
        
        currentSubset.pop();
        used.remove(elem);
    }
    return false;
}

int main() {
    std::cout << "Введите элементы множества через пробел:" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    std::string inputLine;
    std::getline(std::cin, inputLine);
    
    if (inputLine.empty()) {
        std::cout << "Ошибка: пустой ввод!" << std::endl;
        return 1;
    }
    
    HashSet elements;
    Strarray inputOrder;
    std::istringstream iss(inputLine);
    std::string token;
    
    while (iss >> token) {
        try {
            int num = toInt(token);
            if (num <= 0) {
                std::cout << "Ошибка: все числа должны быть натуральными (больше 0). Найдено: " << num << std::endl;
                return 1;
            }
            elements.add(token);
            inputOrder.MPUSH_end(token); 
        } catch (...) {
            std::cout << "Ошибка: некорректное число: " << token << std::endl;
            return 1;
        }
    }
    
    if (elements.size() == 0) {
        std::cout << "Ошибка: не введено ни одного числа!" << std::endl;
        return 1;
    }
    
    std::cout << "\nВведите целевую сумму подмножеств:" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    int targetSum;
    std::cin >> targetSum;
    
    if (targetSum <= 0) {
        std::cout << "Ошибка: сумма должна быть положительной!" << std::endl;
        return 1;
    }
    
    int totalSum = sumHashSet(elements);
    
    std::cout << "\nИсходное множество: " << "{";
    for (int i = 0; i < inputOrder.MSIZE(); ++i) {
        std::cout << inputOrder.MGET_value(i);
        if (i < inputOrder.MSIZE() - 1) std::cout << ", ";
    }
    std::cout << "}" << std::endl;
    std::cout << std::endl;
    std::cout << "Общая сумма элементов: " << totalSum << std::endl;
    std::cout << "Целевая сумма подмножеств: " << targetSum << std::endl;
    
    if (totalSum % targetSum != 0) {
        std::cout << "\nОшибка: общая сумма (" << totalSum 
                  << ") не делится на целевую сумму (" << targetSum << ")." << std::endl;
        std::cout << "Разбиение невозможно!" << std::endl;
        return 1;
    }
    
    int totalSubsets = totalSum / targetSum;
    std::cout << "Ожидаемое количество подмножеств: " << totalSubsets << std::endl;
   
    HashSet used;           
    Stack currentSubset;   
    Strarray result;       
   
    bool found = backtrack(inputOrder, used, targetSum, currentSubset, 0, result, 0, totalSubsets);

    if (found) {
        std::cout << "\n Разбиение на " << totalSubsets 
                  << " подмножества с суммой " << targetSum << std::endl;
        for (int i = 0; i < result.MSIZE(); ++i) {
            std::cout << "Подмножество " << (i + 1) << ": " << result.MGET_value(i) << std::endl;
        }
    } else {
        std::cout << "\nОшибка: множество не может быть разбито на подмножества с суммой " 
                  << targetSum << "." << std::endl;
    }
    
    return 0;
}