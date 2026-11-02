#ifndef ARRAY_H
#define ARRAY_H

#include <string>

class Strarray {
private:
    std::string* data;
    int capacity;
    int length;

    void resize(int new_capacity);

public:
    Strarray();
    ~Strarray();

    void MPUSH_end(const std::string& value);
    void MPUSH_index(int index, const std::string& value);
    void MGET_index(int index) const;
    void MDEL(int index);
    void MREPLACE(int index, const std::string& value);
    int MSIZE() const;
    void MREAD() const;

    void MSAVETOFILE(const std::string& filename) const;
    void MREADFROMFILE(const std::string& filename);
};

#endif