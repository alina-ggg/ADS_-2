// 7 вариант
#include <iostream>
#include <string>
#include "lab1/array.h" // Strarray для хранения алфавита и результатов
#include "lab1/stack.h" // Stack для проверки корректности XML

bool isCorrectXML(const std::string& s) {
    Stack stack;
    int i = 0; // индекс текущего символа в строке
    int n = s.length(); // длина строки

    while (i < n) {
        // пропускаем пробелы
        if (s[i] == ' ') {
            ++i;
            continue;
        }
        if (s[i] != '<') return false;
        ++i;
        if (i >= n) return false;

        bool isClosing = false;
        if (s[i] == '/') {
            isClosing = true;
            ++i;
        }

        std::string tagName = "";
        while (i < n && s[i] != '>') {
            if (s[i] < 'a' || s[i] > 'z') return false;
            tagName += s[i];
            ++i;
        }

        if (tagName.empty()) return false;
        if (i >= n) return false;
        ++i;

        if (isClosing) {
            // закрывающийся теш
            // проверяем что стек не пуст
            if (stack.isEmpty()) return false;
            // достаем последний открытый тег из стека
            // имена открывающего и закрывающего тегов должны совпадать
            if (stack.pop() != tagName) return false;
            } else {
                // открывающийся тег
                stack.push(tagName);
            }
    }
    // в результате все октрытые теги должны быть закрыты - стек должен быть пуст
    return stack.isEmpty();
}

int main() {
    std::cout << "Введите XML-строку:" << std::endl;
    std::cout.flush();

    std::string s;
    std::getline(std::cin, s);

    if (s.empty()) {
        std::cout << "Строка пуста!" << std::endl;
        return 1;
    }

    Strarray alphabet;
    for (char c = 'a'; c <= 'z'; ++c) {
        alphabet.MPUSH_end(std::string(1, c));
    }
    alphabet.MPUSH_end("<");
    alphabet.MPUSH_end(">");
    alphabet.MPUSH_end("/");
    alphabet.MPUSH_end(" ");

    int n = s.length();
    Strarray results; // хранение всех найденных вариантов
    // перебираем каждую позицию в строке - ищем место ошибки
    for (int i = 0; i < n; ++i) {
        char original = s[i]; // запоминание оригинального символа

        // перебор всех символов алфавита из Strarray
        for (int j = 0; j < alphabet.MSIZE(); ++j) {
            std::string rep = alphabet.MGET_value(j);
            if (rep.empty()) continue;

            char c = rep[0];
            if (c == original) continue;

            s[i] = c;

            if (isCorrectXML(s)) {
                results.MPUSH_end(s);
            }
        }
        s[i] = original;
    }

    if (results.MSIZE() == 0) {
        std::cout << "Incorrect" << std::endl;
    } else {
        std::cout << "Найдено " << results.MSIZE() << " вариант(а/ов):" << std::endl;
    
        for (int k = 0; k < results.MSIZE(); ++k) {
            std::cout << results.MGET_value(k) << std::endl;
        }
    }
    return 0;
}