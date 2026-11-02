#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "array.h"
#include "singly_linked_list.h"
#include "doubly_linked_list.h"
#include "stack.h"
#include "queue.h"
#include "complete_binary_tree.h"

std::vector<std::string> splitQuery(const std::string& query) {
    std::vector<std::string> tokens;
    std::stringstream ss(query);
    std::string word;
    while (ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Usage: ./dbms --file <filename> --query \"<COMMAND> <NAME> [args]\"\n";
        return 1;
    }

    std::string filename = "";
    std::string query = "";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[i + 1];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[i + 1];
        }
    }

    std::vector<std::string> tokens = splitQuery(query);
    if (tokens.empty()) {
        std::cout << "Ошибка: Пустой запрос.\n";
        return 1;
    }

    std::string command = tokens[0];
    // tokens[1] — это имя структуры (mystack, myarray, mytree)

    // массив (M)
    if (command == "MPUSH" && tokens.size() >= 3) {
        Strarray arr;
        arr.MREADFROMFILE(filename);
        if (tokens.size() >= 5 && tokens[2] == "index") {
            arr.MPUSH_index(std::stoi(tokens[3]), tokens[4]);
        } else {
            arr.MPUSH_end(tokens[2]);
        }
        arr.MSAVETOFILE(filename);
    }
    else if (command == "MGET" && tokens.size() >= 3) {
        Strarray arr;
        arr.MREADFROMFILE(filename);
        arr.MGET_index(std::stoi(tokens[2]));
    }
    else if (command == "MDEL" && tokens.size() >= 3) {
        Strarray arr;
        arr.MREADFROMFILE(filename);
        arr.MDEL(std::stoi(tokens[2]));
        arr.MSAVETOFILE(filename);
    }
    else if (command == "MREPLACE" && tokens.size() >= 4) {
        Strarray arr;
        arr.MREADFROMFILE(filename);
        arr.MREPLACE(std::stoi(tokens[2]), tokens[3]);
        arr.MSAVETOFILE(filename);
    }
    else if (command == "MSIZE" && tokens.size() >= 2) {
        Strarray arr;
        arr.MREADFROMFILE(filename);
        std::cout << arr.MSIZE() << "\n";
    }
    else if (command == "MREAD" && tokens.size() >= 2) {
        Strarray arr;
        arr.MREADFROMFILE(filename);
        arr.MREAD();
    }

    // односвязный список (F)
    else if (command == "FPUSH" && tokens.size() >= 3) {
        Node* head = FLOAD(filename);
        if (tokens.size() >= 4 && tokens[2] == "tail") {
            head = FPUSHTAIL(head, tokens[3]);
        } else if (tokens.size() >= 5 && tokens[2] == "before") {
            head = FPUSHBEFORE(head, tokens[3], tokens[4]);
        } else if (tokens.size() >= 5 && tokens[2] == "after") {
            head = FPUSHAFTER(head, tokens[3], tokens[4]);
        } else {
            head = FPUSHHEAD(head, tokens[2]);
        }
        FSAVETOFILE(head, filename);
    }
    else if (command == "FDEL" && tokens.size() >= 3) {
        Node* head = FLOAD(filename);
        if (tokens[2] == "head") {
            head = FDELHEAD(head);
        } else if (tokens[2] == "tail") {
            head = FDELTAIL(head);
        } else {
            head = FDELVALUE(head, tokens[2]);
        } 
        FSAVETOFILE(head, filename);
    }
    else if (command == "FSEARCH" && tokens.size() >= 3) {
        Node* head = FLOAD(filename);
        FSEARCH(head, tokens[2]);
    }
    else if (command == "FDELAFTER" && tokens.size() >= 3) {
        Node* head = FLOAD(filename);
        head = FDELAFTER(head, tokens[2]);
        FSAVETOFILE(head, filename);
    }
    else if (command == "FDELBEFORE" && tokens.size() >= 3) {
        Node* head = FLOAD(filename);
        head = FDELBEFORE(head, tokens[2]);
        FSAVETOFILE(head, filename);
    }
    // двусвязный список (L)
    else if (command == "LPUSH" && tokens.size() >= 3) {
        DNode* head = LLOAD(filename);
        DNode* tail = head;
        if (tokens.size() >= 4 && tokens[2] == "tail") {
            DNode* tail = head;
            if (tail) {
                while (tail->next) {
                    tail = tail->next;
                }
            }
            LPUSHTAIL(head, tail, tokens[3]);
        } else if (tokens.size() >= 5 && tokens[2] == "before") {
            DNode* tail = head;
            if (tail) {
                while (tail->next) tail = tail->next;
            }
            head = LPUSHBEFORE(head, tail, tokens[3], tokens[4]);
        } else if (tokens.size() >= 5 && tokens[2] == "after") {
            DNode* tail = head;
            if (tail) {
                while (tail->next) {
                    tail = tail->next;
                }
            }
            head = LPUSHAFTER(head, tail, tokens[3], tokens[4]);
        } else {
            head = LPUSHHEAD(head, tail, tokens[2]);
        }
        LSAVETOFILE(head, filename);
    }
    else if (command == "LDEL" && tokens.size() >= 3) {
        DNode* head = LLOAD(filename);
        if (tokens.size() >= 3 && tokens[2] == "head") {
            DNode* tail = head;
            if (tail) {
                while (tail->next) tail = tail->next;
            }
            head = LDELHEAD(head, tail);
        } else if (tokens[2] == "tail") {
            DNode* tail = head;
            if (tail) {
                while (tail->next) {
                    tail = tail->next;
                }
            }
            head = LDELTAIL(head, tail);
        } else if (tokens.size() >= 4 && tokens[2] == "value") {
            DNode* tail = head;
            if (tail) {
                while (tail->next) tail = tail->next;
            }
            head = LDELVALUE(head, tail, tokens[3]);
        }
        LSAVETOFILE(head, filename);
    }
    else if (command == "LSEARCH" && tokens.size() >= 3) {
        DNode* head = LLOAD(filename);
        LSEARCH(head, tokens[2]);
    }
    

    // стек (S) 
    else if (command == "SPUSH" && tokens.size() >= 3) {
        Stack st;
        st.loadFromFile(filename);
        st.push(tokens[2]);
        st.saveToFile(filename);
    }
    else if (command == "SPOP" && tokens.size() >= 2) {
        Stack st;
        st.loadFromFile(filename);
        std::string val = st.pop();
        std::cout << "-> " << val << "\n";
        st.saveToFile(filename);
    }

    // очередь (Q)
    else if (command == "QPUSH" && tokens.size() >= 3) {
        Queue q;
        q.LOADFROMFILE(filename);
        q.SPUSH(tokens[2]);
        q.SAVETOFILE(filename);
    }
    else if (command == "QPOP" && tokens.size() >= 2) {
        Queue q;
        q.LOADFROMFILE(filename);
        std::string val = q.SPOP();
        std::cout << "-> " << val << "\n";
        q.SAVETOFILE(filename);
    }

    // дерево (Complete Binary Tree - T)
    else if (command == "TINSERT" && tokens.size() >= 3) {
        CompleteBinaryTree tree;
        tree.loadFromFile(filename);
        tree.insert(std::stoi(tokens[2])); // tokens[1] - имя дерева, tokens[2] - число
        tree.saveToFile(filename);
    }
    else if ((command == "TSEARCH" || command == "ISMEMBER") && tokens.size() >= 3) {
        CompleteBinaryTree tree;
        tree.loadFromFile(filename);
        if (tree.search(std::stoi(tokens[2]))) {
            std::cout << "-> TRUE\n";
        } else {
            std::cout << "-> FALSE\n";
        }
    }
    else if (command == "TCHECK" && tokens.size() >= 2) {
        CompleteBinaryTree tree;
        tree.loadFromFile(filename);
        if (tree.isComplete()) {
            std::cout << "-> TRUE\n";
        } else {
            std::cout << "-> FALSE\n";
        }
    }

    // универсальный вывод (PRINT)
    // PRINT M myarray 
    else if (command == "PRINT" && tokens.size() >= 3) {
        std::string type = tokens[1];
        // tokens[2] — имя структуры
        if (type == "M") {
            Strarray arr;
            arr.MREADFROMFILE(filename);
            arr.MREAD();
        } else if (type == "F") {
            Node* head = FLOAD(filename);
            print(head);
        } else if (type == "L") {
            DNode* head = LLOAD(filename);
            print(head);
        } else if (type == "S") {
            Stack st;
            st.loadFromFile(filename);
            st.print();
        } else if (type == "Q") {
            Queue q;
            q.LOADFROMFILE(filename);
            q.PRINT();
        } else if (type == "T") {
            CompleteBinaryTree tree;
            tree.loadFromFile(filename);
            tree.printInOrder();
        }
    }
    else {
        std::cout << "Неизвестная команда или неверные аргументы.\n";
    }

    return 0;
}