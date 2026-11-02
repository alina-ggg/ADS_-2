#include "stack.h"
#include <fstream>
#include <iostream>
#include <vector>

Stack::Stack() : top(nullptr) {}

Stack::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

void Stack::push(const std::string& value) {
    top = new StackNode{value, top};
}

std::string Stack::pop() {
    if (isEmpty()) return "";
    StackNode* temp = top;
    std::string val = top->data;
    top = top->next;
    delete temp;
    return val;
}

bool Stack::isEmpty() const {
    return top == nullptr;
}

void Stack::print() const {
    StackNode* curr = top;
    while (curr) {
        std::cout << curr->data << " ";
        curr = curr->next;
    }
    std::cout << std::endl;
}

void Stack::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    StackNode* curr = top;
    while (curr) {
        file << curr->data << "\n";
        curr = curr->next;
    }
}

void Stack::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    while (!isEmpty()) {
        pop();
    }

    std::vector<std::string> lines;
    std::string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    for (int i = static_cast<int>(lines.size()) - 1; i >= 0; --i) {
        push(lines[i]);
    }
}