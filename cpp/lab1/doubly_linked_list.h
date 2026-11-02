#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <string>

struct DNode {
    std::string data;
    DNode* prev;
    DNode* next;
};

DNode* LPUSHHEAD(DNode*& head, DNode*& tail, const std::string& value);
void LPUSHTAIL(DNode*& head, DNode*& tail, const std::string& value);
DNode* LPUSHBEFORE(DNode*& head, DNode*& tail, const std::string& target, const std::string& value);
DNode* LPUSHAFTER(DNode*& head, DNode*& tail, const std::string& target, const std::string& value);
DNode* LDELHEAD(DNode*& head, DNode*& tail);
DNode* LDELTAIL(DNode*& head, DNode*& tail);
DNode* LDELVALUE(DNode*& head, DNode*& tail, const std::string& value);
DNode* LDELBEFORE(DNode*& head, DNode*& tail, const std::string& target);
DNode* LDELAFTER(DNode*& head, DNode*& tail, const std::string& target);
DNode* LSEARCH(DNode* head, const std::string& value);
void print(DNode* head);
void LSAVETOFILE(DNode* head, const std::string& filename);
DNode* LLOAD(const std::string& filename);

#endif