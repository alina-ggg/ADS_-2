#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <string>

struct Node {
    std::string data;
    Node* next;
};

Node* FPUSHHEAD(Node* head, const std::string& value);
Node* FPUSHTAIL(Node* head, const std::string& value);
Node* FPUSHBEFORE(Node* head, const std::string& target, const std::string& value);
Node* FPUSHAFTER(Node* head, const std::string& target, const std::string& value);

Node* FDELHEAD(Node* head);
Node* FDELTAIL(Node* head);
Node* FDELVALUE(Node* head, const std::string& value);
Node* FDELBEFORE(Node* head, const std::string& target);
Node* FDELAFTER(Node* head, const std::string& target);

void FSEARCH(Node* head, const std::string& value);
void print(Node* head);

void FSAVETOFILE(Node* head, const std::string& filename);
Node* FLOAD(const std::string& filename);

#endif