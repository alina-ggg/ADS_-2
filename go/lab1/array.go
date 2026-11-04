package lab1

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Strarray struct {
	data []string
}

func NewStrarray() *Strarray { return &Strarray{data: make([]string, 0, 10)} }

func (arr *Strarray) MPUSH_end(value string) {
	arr.data = append(arr.data, value)
}

func (arr *Strarray) MPUSH_index(index int, value string) {
	if index < 0 || index > len(arr.data) {
		return
	}
	arr.data = append(arr.data, "")
	copy(arr.data[index+1:], arr.data[index:])
	arr.data[index] = value
}

func (arr *Strarray) MGET_index(index int) {
	if index < 0 || index >= len(arr.data) {
		fmt.Println("Ошибка: Индекс вне диапазона")
		return
	}
	fmt.Println(arr.data[index])
}

func (arr *Strarray) MVALUE_index(index int) string {
	if index < 0 || index >= len(arr.data) {
		return ""
	}
	return arr.data[index]
}

func (arr *Strarray) MDEL(index int) {
	if index < 0 || index >= len(arr.data) {
		return
	}
	arr.data = append(arr.data[:index], arr.data[index+1:]...)
}

func (arr *Strarray) MREPLACE(index int, value string) {
	if index < 0 || index >= len(arr.data) {
		return
	}
	arr.data[index] = value
}

func (arr *Strarray) MSIZE() int { return len(arr.data) }

func (arr *Strarray) MREAD() {
	for _, value := range arr.data {
		fmt.Print(value, " ")
	}
	fmt.Println()
}

func (arr *Strarray) MSAVETOFILE(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	if _, err := fmt.Fprintln(file, len(arr.data)); err != nil {
		return err
	}
	for _, value := range arr.data {
		if _, err := fmt.Fprintln(file, value); err != nil {
			return err
		}
	}
	return nil
}

func (arr *Strarray) MREADFROMFILE(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	scanner := bufio.NewScanner(file)
	if !scanner.Scan() {
		return scanner.Err()
	}
	count, err := strconv.Atoi(strings.TrimSpace(scanner.Text()))
	if err != nil {
		return err
	}
	arr.data = arr.data[:0]
	for i := 0; i < count && scanner.Scan(); i++ {
		arr.MPUSH_end(strings.TrimSpace(scanner.Text()))
	}
	return scanner.Err()
}

func (arr *Strarray) Values() []string {
	result := make([]string, len(arr.data))
	copy(result, arr.data)
	return result
}
