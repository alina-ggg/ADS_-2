// 1 вариант — LRU-кэш
#include "lab1/doubly_linked_list.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class LRUCache {
private:
    int capacity;
    DNode* head;
    DNode* tail;
    std::unordered_map<int, DNode*> nodeByKey;

    static std::string makeNodeData(int key, int value) {
        return std::to_string(key) + ":" + std::to_string(value);
    }

    static int extractKey(const DNode* node) {
        const std::size_t separator = node->data.find(':');
        return std::stoi(node->data.substr(0, separator));
    }

    static int extractValue(const DNode* node) {
        const std::size_t separator = node->data.find(':');
        return std::stoi(node->data.substr(separator + 1));
    }

    // Отсоединение узла от списка
    void detachNode(DNode* node) {
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }

        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }

        node->prev = nullptr;
        node->next = nullptr;
    }

    // Перемещение узла в начало списка
    void moveToFront(DNode* node) {
        if (node == head) {
            return;
        }

        detachNode(node);
        node->next = head;

        if (head != nullptr) {
            head->prev = node;
        } else {
            tail = node;
        }

        head = node;
    }

    // Удаление наименее недавно использованного элемента
    void removeLeastRecentlyUsed() {
        if (tail == nullptr) {
            return;
        }

        DNode* nodeToRemove = tail;
        const int oldKey = extractKey(nodeToRemove);

        detachNode(nodeToRemove);
        nodeByKey.erase(oldKey);

        delete nodeToRemove;
    }

public:
    explicit LRUCache(int maxCapacity)
        : capacity(maxCapacity), head(nullptr), tail(nullptr) {}

    ~LRUCache() {
        while (head != nullptr) {
            DNode* nextNode = head->next;
            delete head;
            head = nextNode;
        }

        tail = nullptr;
        nodeByKey.clear();
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    // Получение значения по ключу.
    int get(int key) {
        const auto found = nodeByKey.find(key);

        if (found == nodeByKey.end()) {
            return -1;
        }

        DNode* node = found->second;
        moveToFront(node);

        return extractValue(node);
    }

    // Добавление или обновление элемента
    void set(int key, int value) {
        if (capacity <= 0) {
            return;
        }

        const auto found = nodeByKey.find(key);

        // Если ключ уже существует, обновляем его значение
        if (found != nodeByKey.end()) {
            DNode* existingNode = found->second;

            existingNode->data = makeNodeData(key, value);
            moveToFront(existingNode);

            return;
        }

        // Если кэш заполнен, удаляем наименее используемый элемент
        if (static_cast<int>(nodeByKey.size()) >= capacity) {
            removeLeastRecentlyUsed();
        }

        // Добавляем новый элемент в начало списка
        DNode* newNode = LPUSHHEAD(
            head,
            tail,
            makeNodeData(key, value)
        );

        nodeByKey[key] = newNode;
    }

    std::size_t size() const {
        return nodeByKey.size();
    }

    bool empty() const {
        return nodeByKey.empty();
    }
};

int main() {
    int cacheCapacity;
    int commandCount;

    if (!(std::cin >> cacheCapacity >> commandCount) ||
        cacheCapacity < 0 || commandCount < 0) {

        std::cerr << "Некорректные параметры LRU-кэша.\n";
        return 1;
    }

    LRUCache cache(cacheCapacity);

    // Сохраняем результаты GET до завершения всех команд
    std::vector<int> results;

    for (int commandIndex = 0;
         commandIndex < commandCount;
         ++commandIndex) {

        std::string command;
        int key;

        if (!(std::cin >> command >> key)) {
            std::cerr << "Не удалось прочитать команду кэша.\n";
            return 1;
        }

        if (command == "SET") {
            int value;

            if (!(std::cin >> value)) {
                std::cerr << "Для SET необходимо указать значение.\n";
                return 1;
            }

            cache.set(key, value);
        }
        else if (command == "GET") {
            results.push_back(cache.get(key));
        }
        else {
            std::cerr << "Неизвестная команда кэша: "
                      << command << '\n';
            return 1;
        }
    }

    // Выводим все результаты GET через пробел
    for (std::size_t i = 0; i < results.size(); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }

        std::cout << results[i];
    }

    std::cout << '\n';

    return 0;
}
