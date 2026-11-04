// 6.1: 1 вариант
// 6.2: 7 вариант
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

bool isPrime(std::size_t number) {
    if (number < 2) {
        return false;
    }

    for (std::size_t divisor = 2; divisor * divisor <= number; ++divisor) {
        if (number % divisor == 0) {
            return false;
        }
    }

    return true;
}

std::size_t nextPrime(std::size_t number) {
    while (!isPrime(number)) {
        ++number;
    }
    return number;
}

// Хеш-таблица с открытой адресацией.
// Для второго способа пробирования шаг вычисляется отдельной хеш-функцией.
class HashTable {
private:
    std::vector<std::string> keys;
    std::vector<bool> occupied;
    bool useDoubleHashing;
    std::size_t collisionCount;

    std::size_t primaryHash(const std::string& value) const {
        return std::hash<std::string>{}(value) % keys.size();
    }

    std::size_t secondaryHash(const std::string& value) const {
        // Размер таблицы простое число, поэтому ненулевой шаг
        // гарантированно взаимно прост с размером таблицы.
        const std::size_t hashValue = std::hash<std::string>{}(value + "#step");
        return 1 + hashValue % (keys.size() - 1);
    }

    std::size_t probeIndex(const std::string& value,
                           std::size_t attempt) const {
        const std::size_t first = primaryHash(value);
        const std::size_t step = useDoubleHashing
            ? secondaryHash(value)
            : 1;

        return (first + attempt * step) % keys.size();
    }

public:
    explicit HashTable(std::size_t expectedElements, bool doubleHashing)
        : keys(nextPrime(expectedElements * 2 + 3)),
          occupied(keys.size(), false),
          useDoubleHashing(doubleHashing),
          collisionCount(0) {}

    bool insert(const std::string& value) {
        for (std::size_t attempt = 0; attempt < keys.size(); ++attempt) {
            const std::size_t index = probeIndex(value, attempt);

            if (!occupied[index]) {
                occupied[index] = true;
                keys[index] = value;
                return true;
            }

            if (keys[index] == value) {
                return false;
            }

            ++collisionCount;
        }

        return false;
    }

    bool contains(const std::string& value) const {
        for (std::size_t attempt = 0; attempt < keys.size(); ++attempt) {
            const std::size_t index = probeIndex(value, attempt);

            if (!occupied[index]) {
                return false;
            }

            if (keys[index] == value) {
                return true;
            }
        }

        return false;
    }

    std::size_t getCollisionCount() const {
        return collisionCount;
    }

    std::size_t capacity() const {
        return keys.size();
    }
};

struct HashExperimentResult {
    bool found;
    std::size_t collisions;
    std::chrono::microseconds elapsed;
    std::size_t tableCapacity;
};

HashExperimentResult runExperiment(const std::vector<std::string>& values,
                                   const std::string& searchValue,
                                   bool doubleHashing) {
    HashTable table(values.size() + 1, doubleHashing);

    const auto startTime = std::chrono::steady_clock::now();

    for (const std::string& value : values) {
        table.insert(value);
    }

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        finishTime - startTime
    ).count();

    return {
        table.contains(searchValue),
        table.getCollisionCount(),
        std::chrono::microseconds(elapsed),
        table.capacity()
    };
}

void printExperiment(const std::string& methodName,
                     const HashExperimentResult& result) {
    std::cout << methodName
              << ": found=" << (result.found ? "true" : "false")
              << ", collisions=" << result.collisions
              << ", capacity=" << result.tableCapacity
              << ", insertion_time=" << result.elapsed.count()
              << " us\n";
}

int runHashTableComparison() {
    int count;
    if (!(std::cin >> count) || count < 0 || count > 100000) {
        std::cerr << "Некорректное количество ключей.\n";
        return 1;
    }

    std::vector<std::string> values(static_cast<std::size_t>(count));
    for (int index = 0; index < count; ++index) {
        if (!(std::cin >> values[static_cast<std::size_t>(index)])) {
            std::cerr << "Не удалось прочитать ключ.\n";
            return 1;
        }
    }

    std::string searchValue;
    if (!(std::cin >> searchValue)) {
        std::cerr << "Не задан ключ для поиска.\n";
        return 1;
    }

    const HashExperimentResult linearResult = runExperiment(
        values, searchValue, false
    );
    const HashExperimentResult doubleResult = runExperiment(
        values, searchValue, true
    );

    printExperiment("Linear probing", linearResult);
    printExperiment("Double hashing", doubleResult);
    return 0;
}

// Разбивает UTF-8-строку на последовательности байтов одного символа.
// Это позволяет считать русские буквы одним символом, а не несколькими байтами.
std::vector<std::string> splitUTF8(const std::string& value) {
    std::vector<std::string> symbols;

    for (std::size_t position = 0; position < value.size();) {
        const unsigned char firstByte =
            static_cast<unsigned char>(value[position]);

        std::size_t symbolLength = 1;
        if ((firstByte & 0x80) == 0) {
            symbolLength = 1;
        } else if ((firstByte & 0xE0) == 0xC0) {
            symbolLength = 2;
        } else if ((firstByte & 0xF0) == 0xE0) {
            symbolLength = 3;
        } else if ((firstByte & 0xF8) == 0xF0) {
            symbolLength = 4;
        }

        if (position + symbolLength > value.size()) {
            symbolLength = 1;
        }

        symbols.push_back(value.substr(position, symbolLength));
        position += symbolLength;
    }

    return symbols;
}

struct SubstringResult {
    int length;
    std::string substring;
};

SubstringResult findLongestUniqueSubstring(
    const std::vector<std::string>& symbols) {

    std::unordered_map<std::string, int> lastOccurrence;

    int leftBorder = 0;
    int maximumLength = 0;
    int bestStart = 0;

    for (int rightBorder = 0;
         rightBorder < static_cast<int>(symbols.size());
         ++rightBorder) {

        const std::string& currentSymbol =
            symbols[static_cast<std::size_t>(rightBorder)];

        const auto previous = lastOccurrence.find(currentSymbol);

        if (previous != lastOccurrence.end() &&
            previous->second >= leftBorder) {
            leftBorder = previous->second + 1;
        }

        lastOccurrence[currentSymbol] = rightBorder;

        const int currentLength = rightBorder - leftBorder + 1;

        if (currentLength > maximumLength) {
            maximumLength = currentLength;
            bestStart = leftBorder;
        }
    }

    std::string bestSubstring;

    for (int i = bestStart; i < bestStart + maximumLength; ++i) {
        bestSubstring += symbols[static_cast<std::size_t>(i)];
    }

    return {maximumLength, bestSubstring};
}

int runLongestUniqueSubstring() {
    std::string input;
    std::getline(std::cin >> std::ws, input);

    const std::vector<std::string> symbols = splitUTF8(input);

    const SubstringResult result =
        findLongestUniqueSubstring(symbols);

    std::cout << result.length
              << " (" << result.substring << ")\n";

    return 0;
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: hashing 6.1 | hashing 6.2\n";
        return 1;
    }

    const std::string taskNumber = argv[1];

    if (taskNumber == "6.1") {
        return runHashTableComparison();
    }

    if (taskNumber == "6.2") {
        return runLongestUniqueSubstring();
    }

    std::cerr << "Неизвестное задание. Укажите 6.1 или 6.2.\n";
    return 1;
}
