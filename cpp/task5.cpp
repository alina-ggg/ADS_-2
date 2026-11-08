// 7 вариант
#include <iostream>
#include <string>
#include <sstream>
#include "lab1/array.h" // Strarray - хранение введенных чисел

// структура узла бинарного дерева
struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

// вставка элемента в BST
void insertBST(TreeNode*& root, int value) {
    if (root == nullptr) {
        root = new TreeNode(value);
        return;
    }

    TreeNode* curr = root;
    while (true) {
        if (value == curr->value) {
            return;
        } else if (value < curr->value) {
            if (curr->left == nullptr) {
                curr->left = new TreeNode(value);
                return;
            }
            curr = curr->left;
        } else {
            if (curr->right == nullptr) {
                curr->right = new TreeNode(value);
                return;
            }
            curr = curr->right;
        }
    }
}

// вычисление высоты дерева (рекурсивно)
int getHeight(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    
    int leftH = getHeight(root->left);
    int rightH = getHeight(root->right);
    
    return 1 + (leftH > rightH ? leftH : rightH);
}

// освобождение памяти дерева
void freeTree(TreeNode* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    std::cout << "Введите последовательность целых чисел, оканчивающуюся нулём:" << std::endl;
    std::cout << "> ";
    std::cout.flush();
    
    std::string inputLine;
    std::getline(std::cin, inputLine);
    
    Strarray numbersStr;
    std::istringstream iss(inputLine);
    std::string token;
    
    while (iss >> token) {
        numbersStr.MPUSH_end(token);
    }
    
    TreeNode* root = nullptr;
    
    for (int i = 0; i < numbersStr.MSIZE(); ++i) {
        std::string numStr = numbersStr.MGET_value(i);
        int num = std::stoi(numStr);
        
        if (num == 0) break;
        
        insertBST(root, num);
        std::cout << "Число " << num << " обработано" << std::endl;
    }
    
    int height = getHeight(root);
    std::cout << "\nВысота дерева: " << height << std::endl;
    
    freeTree(root);
    return 0;
}