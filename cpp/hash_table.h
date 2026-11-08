#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <string>
#include "lab1/array.h"

const int TABLE_SIZE = 11;
const std::string EMPTY = "";
const std::string DELETED = "#";

int hash1(int key) {
    return ((key % TABLE_SIZE) + TABLE_SIZE) % TABLE_SIZE;
}

int hash2(int key) {
    return 1 + (((key / TABLE_SIZE) % (TABLE_SIZE - 1)) + (TABLE_SIZE - 1)) % (TABLE_SIZE - 1);
}

class HashTableLinear {
private:
    Strarray table;
    
public:
    HashTableLinear() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table.MPUSH_end(EMPTY);
        }
    }
    
    bool insert(int key) {
        std::string keyStr = std::to_string(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int index = (hash1(key) + i) % TABLE_SIZE;
            std::string cell = table.MGET_value(index);
            if (cell == EMPTY || cell == DELETED) {
                table.MREPLACE(index, keyStr);
                return true;
            }
        }
        return false;
    }
    
    int search(int key) {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int index = (hash1(key) + i) % TABLE_SIZE;
            std::string cell = table.MGET_value(index);
            if (cell == EMPTY) return -1;
            if (cell == std::to_string(key)) return index;
        }
        return -1;
    }
    
    bool remove(int key) {
        int index = search(key);
        if (index != -1) {
            table.MREPLACE(index, DELETED);
            return true;
        }
        return false;
    }
    
    void print() const {
        std::cout << "\n=== Хеш-таблица (Линейное апробирование) ===" << std::endl;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            std::string cell = table.MGET_value(i);
            std::cout << "Ячейка " << i << ": ";
            if (cell == EMPTY) std::cout << "пусто";
            else if (cell == DELETED) std::cout << "удалено";
            else std::cout << cell;
            std::cout << std::endl;
        }
    }
};

class HashTableDouble {
private:
    Strarray table;
    
public:
    HashTableDouble() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table.MPUSH_end(EMPTY);
        }
    }
    
    bool insert(int key) {
        std::string keyStr = std::to_string(key);
        int h1 = hash1(key);
        int h2 = hash2(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int index = (h1 + i * h2) % TABLE_SIZE;
            std::string cell = table.MGET_value(index);
            if (cell == EMPTY || cell == DELETED) {
                table.MREPLACE(index, keyStr);
                return true;
            }
        }
        return false;
    }
    
    int search(int key) {
        int h1 = hash1(key);
        int h2 = hash2(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int index = (h1 + i * h2) % TABLE_SIZE;
            std::string cell = table.MGET_value(index);
            if (cell == EMPTY) return -1;
            if (cell == std::to_string(key)) return index;
        }
        return -1;
    }
    
    bool remove(int key) {
        int index = search(key);
        if (index != -1) {
            table.MREPLACE(index, DELETED);
            return true;
        }
        return false;
    }
    
    void print() const {
        std::cout << "\n=== Хеш-таблица (Двойное хеширование) ===" << std::endl;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            std::string cell = table.MGET_value(i);
            std::cout << "Ячейка " << i << ": ";
            if (cell == EMPTY) std::cout << "пусто";
            else if (cell == DELETED) std::cout << "удалено";
            else std::cout << cell;
            std::cout << std::endl;
        }
    }
};

const int CHAR_TABLE_SIZE = 128;
const int CHAR_EMPTY = -1;

int charHash(int key) {
    return ((key % CHAR_TABLE_SIZE) + CHAR_TABLE_SIZE) % CHAR_TABLE_SIZE;
}

class CharHashTable {
private:
    int table[CHAR_TABLE_SIZE];
    
public:
    CharHashTable() {
        for (int i = 0; i < CHAR_TABLE_SIZE; ++i) {
            table[i] = CHAR_EMPTY;
        }
    }
    
    void insert(int charCode, int position) {
        int index = charHash(charCode);
        table[index] = position;
    }
    
    int search(int charCode) {
        int index = charHash(charCode);
        return table[index];
    }
    
    void remove(int charCode) {
        int index = charHash(charCode);
        table[index] = CHAR_EMPTY;
    }
    
    bool contains(int charCode) {
        return search(charCode) != CHAR_EMPTY;
    }
};

#endif