#include "doubly_linked_list.h"
#include <iostream>
#include <fstream>

DNode* LPUSHHEAD(DNode*& head, DNode*& tail, const std::string& value) {
    DNode* newNode = new DNode{value, nullptr, head};
    if (head) {
        head->prev = newNode;
    } else {
        tail = newNode;
    }
    return newNode;
}

void LPUSHTAIL(DNode*& head, DNode*& tail, const std::string& value) {
    DNode* newNode = new DNode{value, tail, nullptr};

    if (!head) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

DNode* LPUSHBEFORE(DNode*& head, DNode*& tail, const std::string& target, const std::string& value) {
    if (!head) return nullptr;
    if (head->data == target) return LPUSHHEAD(head, tail, value);
    DNode* curr = head;
    while (curr && curr->data != target) curr = curr->next;
    if (curr) {
        DNode* newNode = new DNode{value, curr->prev, curr};
        curr->prev->next = newNode;
        curr->prev = newNode;
    } 
    return head;
}

DNode* LPUSHAFTER(DNode*& head, DNode*& tail, const std::string& target, const std::string& value) {
    DNode* curr = head;
    while (curr && curr->data != target) curr = curr->next;
    if (curr) {
        DNode* newNode = new DNode{value, curr, curr->next};
        if (curr == tail) {
            LPUSHTAIL(head, tail, value);
        } else {
            DNode* newMode = new DNode{value, curr, curr->next};
            curr->next->prev = newNode;
            curr->next = newMode;
        }
    } 
    return head;
}

DNode* LDELHEAD(DNode*& head, DNode*& tail) {
    if (!head) return nullptr;
    DNode* temp = head->next;
    if (temp) {
        temp->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete head;
    return temp;
}

// Удаление узла, стоящего до целевого значения
DNode* LDELBEFORE(DNode*& head, DNode*& tail, const std::string& target) {
    if (!head) return nullptr;

    DNode* curr = head;
    while (curr && curr->data != target) {
        curr = curr->next;
    }

    if (curr && curr->prev) {
        DNode* temp = curr->prev;
        if (temp == head) {
            return LDELHEAD(head, tail); // Если удаляемый узел был головой
        }

        curr->prev = temp->prev;
        if (temp->prev) {
            temp->prev->next = curr;
        }
        delete temp;
    }
    return head;
}

// Удаление узла, стоящего после целевого значения
DNode* LDELAFTER(DNode*& head, DNode*& tail, const std::string& target) {
    if (!head) return nullptr;

    DNode* curr = head;
    while (curr && curr->data != target) {
        curr = curr->next;
    }

    if (curr && curr->next) {
        DNode* temp = curr->next;
        curr->next = temp->next;
        if (temp == tail) {
            tail = curr;
            tail->next = nullptr;
            delete temp;
        } else {
            curr->next = temp->next;
            if (temp->next) {
                temp->next->prev = curr;
            }
        delete temp;
        }
    }
    return head;
}
DNode* LDELTAIL(DNode*& head, DNode*& tail) {
    if (!head) return nullptr;
    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return nullptr;
    }
    DNode* temp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete temp;
    return head;
}

// Удаление по значению
DNode* LDELVALUE(DNode*& head, DNode*& tail, const std::string& value) {
    if (!head) return nullptr;
    if (head->data == value) {
        return LDELHEAD(head, tail);
    }
    DNode* curr = head;
    while (curr && curr->data != value) {
        curr = curr->next;
    }
    if (curr) {
        if (curr == tail) {
            tail = tail->prev;
            if (tail) {
                tail->next = nullptr;
            }
        } else {
            if (curr->next) curr->next->prev = curr->prev;
            if (curr->prev) curr->prev->next = curr->next;
        }
        delete curr;
    }
    return head;
}

// Загрузка из файла
DNode* LLOAD(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return nullptr;
    DNode* head = nullptr;
    DNode* tail = nullptr;
    std::string value;
    while (file >> value) {
        LPUSHTAIL(head, tail, value);
    }
    return head;
}

// Сохранение в файл
void LSAVETOFILE(DNode* head, const std::string& filename) {
    std::ofstream file(filename);
    DNode* curr = head;
    while (curr) {
        file << curr->data << "\n";
        curr = curr->next;
    }
}

// Поиск элемента
DNode* LSEARCH(DNode* head, const std::string& value) {
    DNode* curr = head;
    while (curr) {
        if (curr->data == value) return curr;
        curr = curr->next;
    }
    return nullptr;
}

// Вывод на экран
void print(DNode* head) {
    DNode* curr = head;
    while (curr) {
        std::cout << curr->data << " ";
        curr = curr->next;
    }
    std::cout << "\n";
}