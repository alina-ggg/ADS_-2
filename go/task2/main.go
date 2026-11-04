package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const (
	minValue = 0
	maxValue = 10000
)

// IntegerSet реализует множество прямым адресованием.
// Индекс массива совпадает со значением элемента.
type IntegerSet struct {
	elements [maxValue + 1]bool
	size     int
}

func NewIntegerSet() *IntegerSet {
	return &IntegerSet{}
}

func isValidValue(value int) bool {
	return value >= minValue && value <= maxValue
}

func (set *IntegerSet) Add(value int) bool {
	if !isValidValue(value) || set.elements[value] {
		return false
	}

	set.elements[value] = true
	set.size++
	return true
}

func (set *IntegerSet) Delete(value int) bool {
	if !isValidValue(value) || !set.elements[value] {
		return false
	}

	set.elements[value] = false
	set.size--
	return true
}

func (set *IntegerSet) Contains(value int) bool {
	return isValidValue(value) && set.elements[value]
}

func (set *IntegerSet) Size() int {
	return set.size
}

// executeCommand обрабатывает одну команду множества.
func executeCommand(command string, rawValue string, set *IntegerSet) {
	value, err := strconv.Atoi(rawValue)
	if err != nil || !isValidValue(value) {
		fmt.Fprintln(os.Stderr, "Значение должно быть целым числом от 0 до 10000.")
		return
	}

	switch command {
	case "SETADD":
		set.Add(value)
		fmt.Println("OK")
	case "SETDEL":
		set.Delete(value)
		fmt.Println("OK")
	case "SET_AT":
		fmt.Println(set.Contains(value))
	default:
		fmt.Fprintf(os.Stderr, "Неизвестная команда: %s\n", command)
	}
}

// processInput считывает команды построчно.
func processInput(input *bufio.Scanner, set *IntegerSet) {
	for input.Scan() {
		parts := strings.Fields(input.Text())
		if len(parts) != 2 {
			if len(parts) != 0 {
				fmt.Fprintln(os.Stderr, "Команда должна содержать операцию и один аргумент.")
			}
			continue
		}

		executeCommand(parts[0], parts[1], set)
	}

	if err := input.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка чтения команд:", err)
	}
}

func main() {
	set := NewIntegerSet()
	arguments := os.Args[1:]
	inputWasSpecified := false

	for index := 0; index < len(arguments); index++ {
		switch arguments[index] {
		case "--file":
			if index+1 >= len(arguments) {
				fmt.Fprintln(os.Stderr, "После --file необходимо указать имя файла.")
				return
			}

			fileName := arguments[index+1]
			index++
			file, err := os.Open(fileName)
			if err != nil {
				fmt.Fprintln(os.Stderr, "Не удалось открыть файл:", err)
				return
			}

			processInput(bufio.NewScanner(file), set)
			file.Close()
			inputWasSpecified = true

		case "--query":
			if index+1 >= len(arguments) {
				fmt.Fprintln(os.Stderr, "После --query необходимо указать строку команд.")
				return
			}

			query := arguments[index+1]
			index++
			processInput(bufio.NewScanner(strings.NewReader(query)), set)
			inputWasSpecified = true

		default:
			fmt.Fprintln(os.Stderr, "Неизвестный аргумент:", arguments[index])
			return
		}
	}

	if !inputWasSpecified {
		processInput(bufio.NewScanner(os.Stdin), set)
	}
}
