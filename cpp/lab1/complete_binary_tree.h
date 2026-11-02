#ifndef COMPLETE_BINARY_TREE_H
#define COMPLETE_BINARY_TREE_H

#include <string>
#include <fstream>
#include <iostream>

struct CBTNode {
    int data;
    CBTNode* left;
    CBTNode* right;
};

class CompleteBinaryTree {
private:
    CBTNode* root;

    int countNodes(CBTNode* node) const;
    bool isCompleteUtil(CBTNode* root, int index, int number_nodes) const;
    void clearHelper(CBTNode* node);
    void printInOrderHelper(CBTNode* node) const;

public:
    CompleteBinaryTree();
    ~CompleteBinaryTree();

    void insert(int value);
    bool search(int value) const;
    bool isComplete() const;
    void clear();
    
    // Сохранение и загрузка
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    void printInOrder() const;
};

#endif