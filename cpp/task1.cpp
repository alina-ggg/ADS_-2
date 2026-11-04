// 7 вариант
#include "lab1/stack.h"

#include <cctype>
#include <iostream>
#include <string>

namespace {

const std::string ALPHABET = "abcdefghijklmnopqrstuvwxyz<>/";

bool isValidTagName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    for (char symbol : name) {
        if (symbol < 'a' || symbol > 'z') {
            return false;
        }
    }

    return true;
}

// Проверка правильности XML с использованием стека из ЛР1.
bool isCorrectXML(const std::string& xml) {
    Stack tags;
    std::size_t position = 0;

    while (position < xml.size()) {

        // Пропускаем пробелы и другие пробельные символы.
        if (std::isspace(static_cast<unsigned char>(xml[position]))) {
            ++position;
            continue;
        }

        if (xml[position] != '<') {
            return false;
        }

        std::size_t closingBracket = xml.find('>', position + 1);

        if (closingBracket == std::string::npos) {
            return false;
        }

        std::string tagBody = xml.substr(
            position + 1,
            closingBracket - position - 1
        );

        if (tagBody.empty()) {
            return false;
        }

        bool isClosingTag = (tagBody[0] == '/');

        std::string tagName = isClosingTag
            ? tagBody.substr(1)
            : tagBody;

        if (!isValidTagName(tagName)) {
            return false;
        }

        if (isClosingTag) {

            if (tags.isEmpty()) {
                return false;
            }

            std::string openedTag = tags.pop();

            if (openedTag != tagName) {
                return false;
            }

        } else {
            tags.push(tagName);
        }

        position = closingBracket + 1;
    }

    return tags.isEmpty();
}

// Восстановление XML заменой одного символа.

bool restoreXML(std::string& xml) {

    if (isCorrectXML(xml)) {
        return true;
    }

    // Два прохода:
    // 1. Сначала исправляем символы в закрывающих тегах.
    // 2. Затем проверяем остальные символы.

    for (int pass = 0; pass < 2; ++pass) {

        for (std::size_t position = 0;
             position < xml.size(); ++position) {

            if (std::isspace(
                static_cast<unsigned char>(xml[position]))) {
                continue;
            }

            // Определяем, находится ли позиция
            // внутри имени закрывающего тега.
            bool isClosingName = false;

            for (std::size_t i = 0; i < position; ++i) {
                if (xml[i] == '<') {
                    isClosingName = false;
                }

                if (xml[i] == '/' &&
                    i > 0 && xml[i - 1] == '<') {
                    isClosingName = true;
                }

                if (xml[i] == '>') {
                    isClosingName = false;
                }
            }

            if (pass == 0 && !isClosingName) {
                continue;
            }

            if (pass == 1 && isClosingName) {
                continue;
            }

            char originalSymbol = xml[position];

            for (char replacement : ALPHABET) {

                if (replacement == originalSymbol) {
                    continue;
                }

                xml[position] = replacement;

                if (isCorrectXML(xml)) {
                    return true;
                }
            }

            xml[position] = originalSymbol;
        }
    }

    return false;
}

} // namespace

int main() {

    std::string damagedXML;

    // Считываем всю строку вместе с пробелами.
    if (!std::getline(std::cin, damagedXML)) {
        std::cerr << "Не удалось прочитать XML-строку.\n";
        return 1;
    }

    if (damagedXML.size() < 7 || damagedXML.size() > 1000) {
        std::cerr << "Длина строки должна быть от 7 до 1000 символов.\n";
        return 1;
    }

    if (restoreXML(damagedXML)) {
        std::cout << damagedXML << '\n';
    } else {
        std::cout << "Incorrect\n";
    }

    return 0;
}
