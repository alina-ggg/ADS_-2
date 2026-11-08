// 1 вариант
#include <iostream>
#include <string>
#include <sstream>
#include "lab1/array.h" // Strarray - хранение множества, флагов, результатов
#include "lab1/stack.h" // Stack - backtracking (перебор с возвратом)

// преобразование строки в целое число
int toInt(const std::string& s) {
    return std::stoi(s);
}

// преобразование целого числа в строку
std::string toString(int n) {
    return std::to_string(n);
}

int sumArray(const Strarray& arr) {
    int s = 0;
    for (int i = 0; i < arr.MSIZE(); ++i) {
        s += toInt(arr.MGET_value(i));
    }
    return s;
}

bool backtrack(
    const Strarray& elements, // исходное множество чисел
    Strarray& used, // флаги использования
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
        std::string subsetStr = "";
        Stack tempStack;

        while (!currentSubset.isEmpty()) {
            std::string val = currentSubset.pop();
            tempStack.push(val);
            if (!subsetStr.empty()) {
                subsetStr = val + " " + subsetStr;
            } else {
                subsetStr = val;
            }
        }
        while (!tempStack.isEmpty()) {
            tempStack.pop();
        }
        
        result.MPUSH_end(subsetStr);

        if (backtrack(elements, used, targetSum, currentSubset, 0, result, subsetsFound + 1, totalSubsets)) {
            return true;
        }

        result.MDEL(result.MSIZE() - 1);
        return false;
    }

    for (int i = 0; i < elements.MSIZE(); ++i) {
        // пропускаем уже использованные элементы
        if (used.MGET_value(i) == "1") continue;

        std::string elemStr = elements.MGET_value(i);
        int elemValue = std::stoi(elemStr);
        if (currentSum + elemValue > targetSum) continue;

        used.MREPLACE(i, "1");

        currentSubset.push(elemStr);

        if (backtrack(elements, used, targetSum, currentSubset, currentSum + elemValue, result, subsetsFound, totalSubsets)) {
            return true;
        }
        currentSubset.pop();
        used.MREPLACE(i, "0");
    }
    return false;
}

void printPatrition(const Strarray& subsets, int targetSum) {
    std::cout << "\nРазбиение на подмножетсва с суммой " << targetSum << std::endl;
    std::cout << "Количество подмножеств: " << subsets.MSIZE() << std::endl;

    for (int i = 0; i < subsets.MSIZE(); ++i) {
        std::string subsetStr = subsets.MGET_value(i);

        std::istringstream iss(subsetStr);
        int subsetSum = 0;
        std::string token;
        while (iss >> token) {
            subsetSum += toInt(token);
        }

        std::string formatted = "";
        std::istringstream iss2(subsetStr);
        bool first = true;
        while (iss2 >> token) {
            if (!first) formatted += ", ";
            formatted += token;
            first = false;
        }

        std::cout << "Подмножество " << (i + 1) << ": {" << formatted
                  << "} (сумма: " << subsetSum << ")" << std::endl;
        
    }
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

    Strarray elements;
    std::istringstream iss(inputLine);
    std::string token;

    while (iss >> token) {
        elements.MPUSH_end(token);
    }

    if (elements.MSIZE() == 0) {
        std::cout << "Ошибка: не введено ни одного числа!" << std::endl;
        return 1;
    }

    for (int i = 0; i < elements.MSIZE(); ++i) {
        try{
            int num = toInt(elements.MGET_value(i));
            if (num <= 0) {
                std::cout << "Ошибка: все числа должны быть натуральными (больше 0). Найдено: " << num << std::endl;
                return 1;
            }
        } catch (...) {
            std::cout << "Ошибка: некорректное число: " << elements.MGET_value(i) << std::endl;
            return 1;
        }
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
    
    int totalSum = sumArray(elements);

    std::cout << "\nИсходное множество: {";
    for (int i = 0; i < elements.MSIZE(); ++i) {
        std::cout << elements.MGET_value(i);
        if (i < elements.MSIZE() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;
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
    
    // массив флагов использования элементов
    Strarray used;
    for (int i = 0; i < elements.MSIZE(); ++i) {
        used.MPUSH_end("0");
    }

    // стек для хранения текущего формируемого подмножества
    Stack currentSubset;

    Strarray result;

    bool found = backtrack(elements, used, targetSum, currentSubset, 0, result, 0, totalSubsets);

    if (found) {
        printPatrition(result, targetSum);
    } else {
        std::cout << "\nОшибка: множество не может быть разбито на подмножества с суммой " 
                  << targetSum << "." << std::endl;
    }

    return 0;
}