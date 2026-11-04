// 1 вариант
#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

class IntegerSet {
private:
    std::unordered_set<int> values;

public:
    bool add(int value) {
        return values.insert(value).second;
    }
};

class SetPartitionSolver {
private:
    std::vector<int> numbers;
    std::vector<int> originalOrder;
    std::vector<bool> used;
    std::vector<std::vector<int>> answer;

    int targetSum;
    int groupCount;

    bool buildGroups(int groupIndex,
                     int startIndex,
                     int currentSum,
                     std::vector<int> currentGroup) {

        // Последняя группа состоит из всех оставшихся элементов
        if (groupIndex == groupCount - 1) {
            std::vector<int> lastGroup;
            int lastSum = 0;

            for (std::size_t i = 0; i < numbers.size(); ++i) {
                if (!used[i]) {
                    lastGroup.push_back(numbers[i]);
                    lastSum += numbers[i];
                }
            }

            if (lastSum != targetSum) {
                return false;
            }

            answer.push_back(lastGroup);
            return true;
        }

        // Текущая группа набрала необходимую сумму
        if (currentSum == targetSum) {
            answer.push_back(currentGroup);

            if (buildGroups(groupIndex + 1, 0, 0, {})) {
                return true;
            }

            answer.pop_back();
            return false;
        }

        int previousValue = -1;

        for (int i = startIndex;
             i < static_cast<int>(numbers.size()); ++i) {

            int value = numbers[i];

            if (used[i]) {
                continue;
            }

            if (value == previousValue) {
                continue;
            }

            if (currentSum + value > targetSum) {
                continue;
            }

            previousValue = value;
            used[i] = true;
            currentGroup.push_back(value);

            if (buildGroups(groupIndex,
                            i + 1,
                            currentSum + value,
                            currentGroup)) {
                return true;
            }

            used[i] = false;
            currentGroup.pop_back();
        }

        return false;
    }

public:
    SetPartitionSolver(const std::vector<int>& input, int sum)
        : numbers(input),
          originalOrder(input),
          used(input.size(), false),
          targetSum(sum),
          groupCount(0) {}

    bool solve() {
        if (numbers.empty() || targetSum <= 0) {
            return false;
        }

        int totalSum = std::accumulate(
            numbers.begin(), numbers.end(), 0
        );

        if (totalSum % targetSum != 0) {
            return false;
        }

        groupCount = totalSum / targetSum;

        // Проверяем, существует ли элемент равный целевой сумме
        bool hasSingleElement = false;

        for (int value : numbers) {
            if (value == targetSum) {
                hasSingleElement = true;
                break;
            }
        }

        // Если такой элемент есть сначала рассматриваем наибольшие элементы
        if (hasSingleElement) {
            std::sort(numbers.rbegin(), numbers.rend());
        }

        if (numbers.front() > targetSum) {
            return false;
        }

        if (!buildGroups(0, 0, 0, {})) {
            return false;
        }

        // Возвращаем элементы каждой группы в исходный порядок ввода
        for (auto& group : answer) {
            std::sort(
                group.begin(),
                group.end(),
                [this](int a, int b) {
                    auto first = std::find(
                        originalOrder.begin(),
                        originalOrder.end(), a
                    );

                    auto second = std::find(
                        originalOrder.begin(),
                        originalOrder.end(), b
                    );

                    return first < second;
                }
            );
        }

        return true;
    }

    const std::vector<std::vector<int>>& getAnswer() const {
        return answer;
    }
};

int main() {
    int count;

    if (!(std::cin >> count) || count <= 0 || count > 30) {
        std::cerr << "Количество элементов должно быть от 1 до 30.\n";
        return 1;
    }

    IntegerSet uniqueNumbers;
    std::vector<int> numbers;

    for (int i = 0; i < count; ++i) {
        int value;

        if (!(std::cin >> value) || value <= 0) {
            std::cerr << "Все элементы должны быть натуральными числами.\n";
            return 1;
        }

        if (uniqueNumbers.add(value)) {
            numbers.push_back(value);
        }
    }

    int targetSum;

    if (!(std::cin >> targetSum)) {
        std::cerr << "Не задана целевая сумма группы.\n";
        return 1;
    }

    SetPartitionSolver solver(numbers, targetSum);

    if (!solver.solve()) {
        std::cout << "Impossible\n";
        return 0;
    }

    const auto& groups = solver.getAnswer();

    for (std::size_t i = 0; i < groups.size(); ++i) {
        if (i > 0) {
            std::cout << " | ";
        }

        std::cout << '{';

        for (std::size_t j = 0; j < groups[i].size(); ++j) {
            if (j > 0) {
                std::cout << ", ";
            }

            std::cout << groups[i][j];
        }

        std::cout << '}';
    }

    std::cout << '\n';

    return 0;
}
