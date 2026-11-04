package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Strarray struct {
	data     []string
	capacity int
	length   int
}

// Конструктор
func NewStrarray() *Strarray {
	return &Strarray{
		data:     make([]string, 10),
		capacity: 10,
		length:   0,
	}
}

// Изменение емкости
func (arr *Strarray) resize(newCapacity int) {
	newData := make([]string, newCapacity)
	copy(newData, arr.data[:arr.length])
	arr.data = newData
	arr.capacity = newCapacity
}

// Добавление в конец
func (arr *Strarray) MPUSH_end(value string) {
	if arr.length == arr.capacity {
		arr.resize(arr.capacity * 2)
	}
	arr.data[arr.length] = value
	arr.length++
}

// Добавление по индексу
func (arr *Strarray) MPUSH_index(index int, value string) {
	if index < 0 || index > arr.length {
		fmt.Println("Ошибка: Индекс вне диапазона")
		return
	}
	if arr.length == arr.capacity {
		arr.resize(arr.capacity * 2)
	}
	// Сдвиг элементов вправо
	for i := arr.length; i > index; i-- {
		arr.data[i] = arr.data[i-1]
	}
	arr.data[index] = value
	arr.length++
}

// Получение элемента по индексу
func (arr *Strarray) MGET_index(index int) {
	if index < 0 || index >= arr.length {
		fmt.Println("Ошибка: Индекс вне диапазона")
		return
	}
	fmt.Println(arr.data[index])
}

// Удаление по индексу
func (arr *Strarray) MDEL(index int) {
	if index < 0 || index >= arr.length {
		fmt.Println("Ошибка: Индекс вне диапазона")
		return
	}
	// Сдвиг элементов влево
	for i := index; i < arr.length-1; i++ {
		arr.data[i] = arr.data[i+1]
	}
	arr.length--
}

// Замена элемента по индексу
func (arr *Strarray) MREPLACE(index int, value string) {
	if index < 0 || index >= arr.length {
		fmt.Println("Ошибка: Индекс вне диапазона")
		return
	}
	arr.data[index] = value
}

// Получение длины массива
func (arr *Strarray) MSIZE() int {
	return arr.length
}

// Чтение (вывод всех элементов)
func (arr *Strarray) MREAD() {
	for i := 0; i < arr.length; i++ {
		fmt.Print(arr.data[i], " ")
	}
	fmt.Println()
}

// Сохранение в файл
func (arr *Strarray) MSAVETOFILE(filename string) {
	file, err := os.Create(filename)
	if err != nil {
		return
	}
	defer file.Close()

	file.WriteString(strconv.Itoa(arr.length) + "\n")
	for i := 0; i < arr.length; i++ {
		file.WriteString(arr.data[i] + "\n")
	}
}

// Чтение из файла
func (arr *Strarray) MREADFROMFILE(filename string) {
	file, err := os.Open(filename)
	if err != nil {
		return
	}
	defer file.Close()

	arr.length = 0
	scanner := bufio.NewScanner(file)

	if scanner.Scan() {
		count, err := strconv.Atoi(strings.TrimSpace(scanner.Text()))
		if err == nil {
			for i := 0; i < count && scanner.Scan(); i++ {
				val := strings.TrimSpace(scanner.Text())
				arr.MPUSH_end(val)
			}
		}
	}
	if err := scanner.Err(); err != nil {
	}
}
func (arr *Strarray) ValueAt(index int) string {
	if index < 0 || index >= arr.length {
		return ""
	}
	return arr.data[index]
}
