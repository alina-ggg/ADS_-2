package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

const allowedSymbols = "abcdefghijklmnopqrstuvwxyz<>/"

// isValidTagName проверяет допустимость имени открывающего
// или закрывающего XML-тега по условиям задания.
func isValidTagName(name string) bool {
	if len(name) == 0 {
		return false
	}

	for _, symbol := range name {
		if symbol < 'a' || symbol > 'z' {
			return false
		}
	}

	return true
}

// isCorrectXML проверяет порядок и парность XML-тегов.
// Для хранения открытых тегов применяется стек из лабораторной №1.
func isCorrectXML(xml string) bool {
	stack := NewStack()
	position := 0

	for position < len(xml) {
		if xml[position] != '<' {
			return false
		}

		relativeEnd := strings.IndexByte(xml[position+1:], '>')
		if relativeEnd < 0 {
			return false
		}

		closingPosition := position + 1 + relativeEnd
		tagBody := xml[position+1 : closingPosition]
		if tagBody == "" {
			return false
		}

		closingTag := tagBody[0] == '/'
		tagName := tagBody
		if closingTag {
			tagName = tagBody[1:]
		}

		if !isValidTagName(tagName) {
			return false
		}

		if closingTag {
			if stack.IsEmpty() {
				return false
			}

			openedName := stack.Pop()
			if openedName != tagName {
				return false
			}
		} else {
			stack.Push(tagName)
		}

		position = closingPosition + 1
	}

	return stack.IsEmpty()
}

// restoreXML перебирает все позиции и все допустимые символы.
// Изменяется ровно один символ исходной строки.
func restoreXML(damaged string) (string, bool) {
	characters := []byte(damaged)

	for position := 0; position < len(characters); position++ {
		originalSymbol := characters[position]

		for index := 0; index < len(allowedSymbols); index++ {
			replacement := allowedSymbols[index]
			if replacement == originalSymbol {
				continue
			}

			characters[position] = replacement
			candidate := string(characters)

			if isCorrectXML(candidate) {
				return candidate, true
			}
		}

		characters[position] = originalSymbol
	}

	return "", false
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Buffer(make([]byte, 1024), 4096)

	if !scanner.Scan() {
		fmt.Fprintln(os.Stderr, "Не удалось прочитать XML-строку.")
		return
	}

	damagedXML := strings.TrimSpace(scanner.Text())
	if len(damagedXML) < 7 || len(damagedXML) > 1000 {
		fmt.Fprintln(os.Stderr, "Длина строки должна быть от 7 до 1000 символов.")
		return
	}

	restoredXML, success := restoreXML(damagedXML)
	if !success {
		fmt.Println("Incorrect")
		return
	}

	fmt.Println(restoredXML)
}
