package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

const (
	minValue = 0
	maxValue = 10000
)

// Множество с прямым адресованием
type IntegerSet struct {
	elements [maxValue + 1]bool
	count    int
}

func (set *IntegerSet) valid(value int) bool { return value >= minValue && value <= maxValue }
func (set *IntegerSet) Add(value int) bool {
	if !set.valid(value) || set.elements[value] {
		return false
	}
	set.elements[value] = true
	set.count++
	return true
}
func (set *IntegerSet) Remove(value int) bool {
	if !set.valid(value) || !set.elements[value] {
		return false
	}
	set.elements[value] = false
	set.count--
	return true
}
func (set *IntegerSet) Contains(value int) bool {
	return set.valid(value) && set.elements[value]
}
func (set *IntegerSet) Size() int { return set.count }

func (set *IntegerSet) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return fmt.Errorf("не удалось открыть файл: %w", err)
	}
	defer file.Close()
	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)
	for scanner.Scan() {
		value, parseErr := strconv.Atoi(scanner.Text())
		if parseErr != nil {
			return fmt.Errorf("ошибка чтения файла: некорректный элемент %q", scanner.Text())
		}
		if !set.Add(value) && !set.valid(value) {
			fmt.Fprintf(os.Stderr, "Некорректный элемент: %d\n", value)
		}
	}
	return scanner.Err()
}

func (set *IntegerSet) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return fmt.Errorf("не удалось сохранить файл: %w", err)
	}
	defer file.Close()
	writer := bufio.NewWriter(file)
	for value := minValue; value <= maxValue; value++ {
		if set.elements[value] {
			if _, err := fmt.Fprintln(writer, value); err != nil {
				return err
			}
		}
	}
	return writer.Flush()
}

func processCommands(input io.Reader, set *IntegerSet) {
	scanner := bufio.NewScanner(input)
	scanner.Split(bufio.ScanWords)
	for scanner.Scan() {
		command := scanner.Text()
		if !scanner.Scan() {
			fmt.Fprintln(os.Stderr, "Не указано значение для команды.")
			break
		}
		rawValue := scanner.Text()
		value, err := strconv.Atoi(rawValue)
		if err != nil || !set.valid(value) {
			fmt.Fprintln(os.Stderr, "Значение должно быть от 0 до 10000.")
			continue
		}
		switch command {
		case "SETADD":
			if set.Add(value) {
				fmt.Println("OK")
			} else {
				fmt.Println("EXISTS")
			}
		case "SETDEL":
			if set.Remove(value) {
				fmt.Println("OK")
			} else {
				fmt.Println("NOT FOUND")
			}
		case "SET_AT":
			if set.Contains(value) {
				fmt.Println("true")
			} else {
				fmt.Println("false")
			}
		default:
			fmt.Fprintln(os.Stderr, "Неизвестная команда:", command)
		}
	}
	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка чтения команд:", err)
	}
}

func main() {
	var filename, query string
	fileSpecified, querySpecified := false, false

	for i := 1; i < len(os.Args); i++ {
		switch os.Args[i] {
		case "--file":
			if i+1 >= len(os.Args) {
				fmt.Fprintln(os.Stderr, "После --file необходимо указать имя файла.")
				os.Exit(1)
			}
			i++
			filename = os.Args[i]
			fileSpecified = true
		case "--query":
			if i+1 >= len(os.Args) {
				fmt.Fprintln(os.Stderr, "После --query необходимо указать запрос.")
				os.Exit(1)
			}
			i++
			query = os.Args[i]
			querySpecified = true
		default:
			fmt.Fprintln(os.Stderr, "Некорректный аргумент:", os.Args[i])
			os.Exit(1)
		}
	}

	set := &IntegerSet{}
	if fileSpecified {
		if _, err := os.Stat(filename); err != nil {
			if !os.IsNotExist(err) {
				fmt.Fprintln(os.Stderr, "Не удалось открыть файл:", err)
				os.Exit(1)
			}
			file, createErr := os.Create(filename)
			if createErr != nil {
				fmt.Fprintln(os.Stderr, "Не удалось создать файл:", createErr)
				os.Exit(1)
			}
			file.Close()
		} else if err := set.LoadFromFile(filename); err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
	}

	if querySpecified {
		processCommands(strings.NewReader(query), set)
	} else {
		processCommands(os.Stdin, set)
	}

	if fileSpecified {
		if err := set.SaveToFile(filename); err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
	}
}
