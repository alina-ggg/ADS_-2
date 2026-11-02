#ifndef STACK_H
#define STACK_H

#include <string>

struct StackNode {
    std::string data;
    StackNode* next;
};

class Stack {
private:
    StackNode* top;
public:
    Stack();
    ~Stack();
    void push(const std::string& value);
    std::string pop();
    bool isEmpty() const;
    void print() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif