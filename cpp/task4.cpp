// 1 вариант
#include <iostream>
#include <string>
#include <sstream>
#include "lab1/array.h" // Strarray - хранение исходного множества и результатов
#include "lab1/stack.h" // Stack - хранение текущего подмножества

// генерация всех подмножеств заданного размера
void generateSubsetsOfSize(
    const Strarray& elements,
    int index,
    int targetSize,
    int currentSize,
    Stack& current,
    Strarray& result
) {
    if (currentSize == targetSize) {
        std::string subsetStr = "";
        Stack tempStack;

        // перекладываем элементы для сохранения порядка
        while (!current.isEmpty()) {
            std::string val = current.pop();
            tempStack.push(val);
        }

        // собираем строку и возвращаем элементы в стек
        while (!tempStack.isEmpty()) {
            std::string val = tempStack.pop();
            current.push(val);
            if (!subsetStr.empty()) {
                subsetStr += ", " + val;
            } else {
                subsetStr = val;
            }
        }

        result.MPUSH_end("{" + subsetStr + "}");
        return;
    }

    if (index == elements.MSIZE()) {
        return;
    }

    current.push(elements.MGET_value(index));
    generateSubsetsOfSize(elements, index + 1, targetSize, currentSize + 1, current, result);
    current.pop();

    generateSubsetsOfSize(elements, index + 1, targetSize, currentSize, current, result);
}

void generateAllSubsets(const Strarray& elements, Strarray& result) {
    int n = elements.MSIZE();
    Stack currentSubset;

    for (int size = 0; size <= n; ++size) {
        generateSubsetsOfSize(elements, 0, size, 0, currentSubset, result);
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
        std::cout << "Ошибка: не введено ни одного элемента!" << std::endl;
        return 1;
    }

    std::cout << "\nВвод: S = {";
    for (int i = 0; i < elements.MSIZE(); ++i) {
        std::cout << elements.MGET_value(i);
        if (i < elements.MSIZE() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "}." << std::endl;

    int n = elements.MSIZE();
    int totalSubset = 1;
    for (int i = 0; i < n; ++i) {
        totalSubset *= 2;
    }

    Strarray result;
    generateAllSubsets(elements, result);

    std::cout << "Вывод: [";
    for (int i = 0; i < result.MSIZE(); ++i) {
        std::cout << result.MGET_value(i);
        if (i < result.MSIZE() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    return 0;
}