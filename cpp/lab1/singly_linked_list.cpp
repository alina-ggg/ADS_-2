#include "singly_linked_list.h"
#include <iostream>
#include <fstream>

Node* FPUSHHEAD(Node* head, const std::string& value) {
    return new Node{value, head};
}

Node* FPUSHTAIL(Node* head, const std::string& value) {
    Node* newNode = new Node{value, nullptr};
    if (!head) return newNode;
    Node* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
    return head;
}

Node* FPUSHBEFORE(Node* head, const std::string& target, const std::string& value) {
    if (!head) return nullptr;
    if (head->data == target) return FPUSHHEAD(head, value);
    Node* curr = head;
    while (curr->next && curr->next->data != target) {
        curr = curr->next;
    }
    if (curr->next) {
        curr->next = new Node{value, curr->next};
    }
    return head;
}

Node* FPUSHAFTER(Node* head, const std::string& target, const std::string& value) {
    Node* curr = head;
    while (curr && curr->data != target) {
        curr = curr->next;
    }
    if (curr) {
        curr->next = new Node{value, curr->next};
    }
    return head;
}

Node* FDELHEAD(Node* head) {
    if (!head) return nullptr;
    Node* temp = head->next;
    delete head;
    return temp;
}

Node* FDELTAIL(Node* head) {
    if (!head) return nullptr;
    if (!head->next) {
        delete head;
        return nullptr;
    }
    Node* curr = head;
    while (curr->next->next) {
        curr = curr->next;
    }
    delete curr->next;
    curr->next = nullptr;
    return head;
}

Node* FDELVALUE(Node* head, const std::string& value) {
    if (!head) return nullptr;
    if (head->data == value) return FDELHEAD(head);
    Node* curr = head;
    while (curr->next && curr->next->data != value) {
        curr = curr->next;
    }
    if (curr->next) {
        Node* temp = curr->next;
        curr->next = curr->next->next;
        delete temp;
    }
    return head;
}

// Удаление узла, стоящего до целевого значения
Node* FDELBEFORE(Node* head, const std::string& target) {
    if (!head || !head->next) return head;

    if (head->next->data == target) {
        return FDELHEAD(head);
    }

    Node* curr = head;
    while (curr->next && curr->next->next) {
        if (curr->next->next->data == target) {
            Node* temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
            return head;
        }
        curr = curr->next;
    }
    return head;
}

Node* FDELAFTER(Node* head, const std::string& target) {
    Node* curr = head;
    while (curr && curr->data != target) {
        curr = curr->next;
    }

    if (curr && curr->next) {
        Node* temp = curr->next;
        curr->next = curr->next->next;
        delete temp;
    }
    return head;
}

void FSEARCH(Node* head, const std::string& value) {
    Node* curr = head;
    int pos = 0;
    while (curr) {
        if (curr->data == value) {
            std::cout << "Элемент найден на позиции: " << pos << "\n";
            return;
        }
        curr = curr->next;
        pos++;
    }
    std::cout << "Элемент не найден\n";
}

void print(Node* head) {
    Node* curr = head;
    while (curr) {
        std::cout << curr->data << " ";
        curr = curr->next;
    }
    std::cout << "\n";
}

void FSAVETOFILE(Node* head, const std::string& filename) {
    std::ofstream file(filename);
    Node* curr = head;
    while (curr) {
        file << curr->data << "\n";
        curr = curr->next;
    }
}

Node* FLOAD(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    Node* head = nullptr;
    Node* tail = nullptr;
    while (getline(file, line)) {
        if (!line.empty()) {
            Node* newNode = new Node{line, nullptr};
            if (!head) head = tail = newNode;
            else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }
    return head;
}