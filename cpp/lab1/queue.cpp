#include "queue.h"
#include <fstream>
#include <iostream>

Queue::Queue() : head(nullptr), tail(nullptr) {}

Queue::~Queue() {
    while (!ISEMPTY()) {
        SPOP();
    }
}

void Queue::SPUSH(const std::string& value) {
    QNode* newNode = new QNode{value, nullptr};
    if (tail) {
        tail->next = newNode;
        tail = newNode;
    } else {
        head = tail = newNode;
    }
}

std::string Queue::SPOP() {
    if (ISEMPTY()) return "";
    QNode* temp = head;
    std::string val = head->data;
    head = head->next;
    if (!head) {
        tail = nullptr;
    }
    delete temp;
    return val;
}

bool Queue::ISEMPTY() const {
    return head == nullptr;
}

void Queue::PRINT() const {
    QNode* curr = head;
    while (curr) {
        std::cout << curr->data << " ";
        curr = curr->next;
    }
    std::cout << std::endl;
}

void Queue::SAVETOFILE(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    QNode* curr = head;
    while (curr) {
        file << curr->data << "\n";
        curr = curr->next;
    }
}

void Queue::LOADFROMFILE(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    while (!ISEMPTY()) {
        SPOP();
    }
    std::string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            SPUSH(line);
        }
    }
}