#include "array.h"
#include <iostream>
#include <fstream>

Strarray::Strarray() : capacity(10), length(0) {
    data = new std::string[capacity];
}

Strarray::~Strarray() {
    delete[] data;
}

void Strarray::resize(int new_capacity) {
    std::string* new_data = new std::string[new_capacity];
    for (int i = 0; i < length; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

void Strarray::MPUSH_end(const std::string& value) {
    if (length == capacity) {
        resize(capacity * 2);
    }
    data[length++] = value;
}

void Strarray::MPUSH_index(int index, const std::string& value) {
    if (index < 0 || index > length) {
        std::cout << "Ошибка: Индекс вне диапазона\n";
        return;
    }
    if (length == capacity) {
        resize(capacity * 2);
    }
    for (int i = length; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    length++;
}

void Strarray::MGET_index(int index) const {
    if (index < 0 || index >= length) {
        std::cout << "Ошибка: Индекс вне диапазона\n";
        return;
    }
    std::cout << data[index] << "\n";
}

void Strarray::MDEL(int index) {
    if (index < 0 || index >= length) {
        std::cout << "Ошибка: Индекс вне диапазона\n";
        return;
    }
    for (int i = index; i < length - 1; ++i) {
        data[i] = data[i + 1];
    }
    length--;
}

void Strarray::MREPLACE(int index, const std::string& value) {
    if (index < 0 || index >= length) {
        std::cout << "Ошибка: Индекс вне диапазона\n";
        return;
    }
    data[index] = value;
}

int Strarray::MSIZE() const {
    return length;
}

void Strarray::MREAD() const {
    for (int i = 0; i < length; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "\n";
}

void Strarray::MSAVETOFILE(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    file << length << "\n";
    for (int i = 0; i < length; ++i) {
        file << data[i] << "\n";
    }
}

void Strarray::MREADFROMFILE(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    length = 0;
    int count = 0;
    if (file >> count) {
        std::string val;
        while (count-- > 0 && file >> val) {
            MPUSH_end(val);
        }
    }
}