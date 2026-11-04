package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"lab2/lab1"
)

const allowedSymbols = "abcdefghijklmnopqrstuvwxyz<>/"

func isValidTagName(name string) bool {
	if name == "" {
		return false
	}
	for _, symbol := range name {
		if symbol < 'a' || symbol > 'z' {
			return false
		}
	}
	return true
}

// Проверка XML использует стек из лабораторной работы №1
func isCorrectXML(xml string) bool {
	tags := lab1.NewStack()
	position := 0

	for position < len(xml) {
		if strings.ContainsRune(" \t\n\r\v\f", rune(xml[position])) {
			position++
			continue
		}
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
			if tags.IsEmpty() || tags.Pop() != tagName {
				return false
			}
		} else {
			tags.Push(tagName)
		}
		position = closingPosition + 1
	}
	return tags.IsEmpty()
}

func restoreXML(xml string) (string, bool) {
	if isCorrectXML(xml) {
		return xml, true
	}
	characters := []byte(xml)

	for pass := 0; pass < 2; pass++ {
		for position := 0; position < len(characters); position++ {
			if strings.ContainsRune(" \t\n\r\v\f", rune(characters[position])) {
				continue
			}

			isClosingName := false
			for i := 0; i < position; i++ {
				if characters[i] == '<' {
					isClosingName = false
				}
				if characters[i] == '/' && i > 0 && characters[i-1] == '<' {
					isClosingName = true
				}
				if characters[i] == '>' {
					isClosingName = false
				}
			}
			if pass == 0 && !isClosingName {
				continue
			}
			if pass == 1 && isClosingName {
				continue
			}

			original := characters[position]
			for i := 0; i < len(allowedSymbols); i++ {
				replacement := allowedSymbols[i]
				if replacement == original {
					continue
				}
				characters[position] = replacement
				if isCorrectXML(string(characters)) {
					return string(characters), true
				}
			}
			characters[position] = original
		}
	}
	return "", false
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Buffer(make([]byte, 1024), 4096)
	if !scanner.Scan() {
		fmt.Fprintln(os.Stderr, "Не удалось прочитать XML-строку.")
		os.Exit(1)
	}
	damagedXML := scanner.Text()
	if len(damagedXML) < 7 || len(damagedXML) > 1000 {
		fmt.Fprintln(os.Stderr, "Длина строки должна быть от 7 до 1000 символов.")
		os.Exit(1)
	}
	restored, ok := restoreXML(damagedXML)
	if !ok {
		fmt.Println("Incorrect")
		return
	}
	fmt.Println(restored)
}
