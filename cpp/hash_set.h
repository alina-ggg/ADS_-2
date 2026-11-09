#ifndef HASH_SET_H
#define HASH_SET_H

#include <string>
#include <iostream>
#include "lab1/array.h"                  
#include "lab1/singly_linked_list.h"

// константы и хэш-функция

const int HASH_SIZE = 1009;

// хеш функция для строки
// преобразует строку в индекс массива
int hashFunction(const std::string& key) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return static_cast<int>(hash % HASH_SIZE);
}

// проверка наличия элемента в списке
bool containsNode(Node* head, const std::string& value) {
    Node* curr = head;
    while (curr != nullptr) {
        if (curr->data == value) return true;
        curr = curr->next;
    }
    return false;
}

class HashSet {
    private:
        Node** buskets; // массив указателей на головы списков (цепочки коллизий)
    public:
        // конструктор: массив пустых списков
        HashSet() {
            buskets = new Node*[HASH_SIZE];
            for (int i = 0; i < HASH_SIZE; ++i) {
                buskets[i] = nullptr;
            }
        }
        // деструктор: освобождение памяти всех списков
        ~HashSet() {
            for (int i = 0; i < HASH_SIZE; ++i) {
                Node* curr = buskets[i];
                while (curr != nullptr) {
                    Node* temp = curr;
                    curr = curr->next;
                    delete temp;
                }
            }
            delete[] buskets;
        }
        
        // SETADD
        void add(const std::string& value) {
            int index = hashFunction(value);
            if (!containsNode(buskets[index], value)) {
                buskets[index] = FPUSHTAIL(buskets[index], value);
            }
        }

        // SETDEL
        void remove(const std::string& value) {
            int index = hashFunction(value);
            buskets[index] = FDELVALUE(buskets[index], value);
        }

        // SET_AT
        bool contains(const std::string& value) {
            int index = hashFunction(value);
            return containsNode(buskets[index], value);
        }
    
        int size() const {
            int count = 0;
            for (int i = 0; i < HASH_SIZE; ++i) {
                Node* curr = buskets[i];
                while (curr != nullptr) {
                    ++count;
                    curr = curr->next;
                }
            }
            return count;
        }
        Strarray getElements() const {
            Strarray res;
            for (int i = 0; i < HASH_SIZE; ++i) {
                Node* curr = buskets[i];
                while (curr != nullptr) {
                    res.MPUSH_end(curr->data);
                    curr = curr->next;
                }
            }
            return res;
        }
        void print() const {
            std::cout << "{";
            bool first = true;
            for (int i = 0; i < HASH_SIZE; ++i) {
                Node* curr = buskets[i];
                while (curr != nullptr) {
                    if (!first) std::cout << ", ";
                    std::cout << curr->data;
                    first = false;
                    curr = curr->next;
                }
            }
            std::cout << "}";
        }
    };

#endif