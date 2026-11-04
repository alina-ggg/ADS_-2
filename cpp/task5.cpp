// 7 вариант
#include <algorithm>
#include <iostream>
#include <queue>

struct TreeNode {
    int key;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(int value)
        : key(value), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    TreeNode* root;

    void destroySubtree(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        destroySubtree(node->left);
        destroySubtree(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroySubtree(root);
    }

    void insert(int value) {
        if (root == nullptr) {
            root = new TreeNode(value);
            return;
        }

        TreeNode* current = root;

        while (true) {
            if (value < current->key) {
                if (current->left == nullptr) {
                    current->left = new TreeNode(value);
                    return;
                }
                current = current->left;
            } else {
                // Повторяющиеся ключи помещаются в правое поддерево
                if (current->right == nullptr) {
                    current->right = new TreeNode(value);
                    return;
                }
                current = current->right;
            }
        }
    }

    int height() const {
        if (root == nullptr) {
            return 0;
        }

        // Обход по уровням не использует рекурсию и не зависит
        // от глубины дерева
        std::queue<const TreeNode*> nodes;
        nodes.push(root);
        int treeHeight = 0;

        while (!nodes.empty()) {
            const std::size_t nodesOnLevel = nodes.size();
            ++treeHeight;

            for (std::size_t index = 0; index < nodesOnLevel; ++index) {
                const TreeNode* current = nodes.front();
                nodes.pop();

                if (current->left != nullptr) {
                    nodes.push(current->left);
                }
                if (current->right != nullptr) {
                    nodes.push(current->right);
                }
            }
        }

        return treeHeight;
    }
};

int main() {
    BinarySearchTree tree;
    int value;

    // Ноль является признаком окончания ввода и в дерево не помещается
    while (std::cin >> value) {
        if (value == 0) {
            break;
        }
        tree.insert(value);
    }

    std::cout << tree.height() << '\n';
    return 0;
}
