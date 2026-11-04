// 1 вариант
#include "lab1/array.h"

#include <iostream>
#include <string>
#include <vector>

class SubsetGenerator {
private:
    const Strarray& source;
    std::vector<std::string> currentSubset;
    std::size_t generatedCount;

    void printCurrentSubset() {
        std::cout << '{';

        for (std::size_t index = 0; index < currentSubset.size(); ++index) {
            if (index > 0) {
                std::cout << ", ";
            }
            std::cout << currentSubset[index];
        }

        std::cout << "}\n";
        ++generatedCount;
    }

    // Рекурсивно выбирает элементы, начиная с указанной позиции.
    void generateFrom(int startIndex, int requiredSize) {
        if (static_cast<int>(currentSubset.size()) == requiredSize) {
            printCurrentSubset();
            return;
        }

        const int remaining = requiredSize - static_cast<int>(currentSubset.size());
        const int lastPossibleIndex = source.MSIZE() - remaining;

        for (int index = startIndex; index <= lastPossibleIndex; ++index) {
            currentSubset.push_back(source.MVALUE_index(index));
            generateFrom(index + 1, requiredSize);
            currentSubset.pop_back();
        }
    }

public:
    explicit SubsetGenerator(const Strarray& array)
        : source(array), generatedCount(0) {}

    void generateAll() {
        // Сначала выводим пустое множество, затем подмножества
        // размером от 1 до N.
        for (int subsetSize = 0; subsetSize <= source.MSIZE(); ++subsetSize) {
            generateFrom(0, subsetSize);
        }
    }

    std::size_t getGeneratedCount() const {
        return generatedCount;
    }
};

int main() {
    int count;
    if (!(std::cin >> count) || count < 0 || count > 25) {
        std::cerr << "Количество элементов должно быть от 0 до 25.\n";
        return 1;
    }

    Strarray inputArray;

    for (int index = 0; index < count; ++index) {
        std::string value;
        if (!(std::cin >> value)) {
            std::cerr << "Не удалось прочитать элемент массива.\n";
            return 1;
        }
        inputArray.MPUSH_end(value);
    }

    SubsetGenerator generator(inputArray);
    generator.generateAll();

    return 0;
}
