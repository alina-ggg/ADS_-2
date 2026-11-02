#ifndef QUEUE_H
#define QUEUE_H

#include <string>

struct QNode {
    std::string data;
    QNode* next;
};

class Queue {
private:
    QNode* head;
    QNode* tail;
public:
    Queue();
    ~Queue();
    void SPUSH(const std::string& value); // Добавление в хвост
    std::string SPOP();                 // Удаление из головы
    bool ISEMPTY() const;
    void PRINT() const;
    void SAVETOFILE(const std::string& filename) const;
    void LOADFROMFILE(const std::string& filename);
};

#endif