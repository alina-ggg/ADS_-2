package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type StackNode struct {
	data string
	next *StackNode
}

type Stack struct {
	top *StackNode
}

// Конструктор
func NewStack() *Stack {
	return &Stack{top: nil}
}

// Добавление элемента на вершину
func (s *Stack) Push(value string) {
	s.top = &StackNode{data: value, next: s.top}
}

// Извлечение элемента с вершины
func (s *Stack) Pop() string {
	if s.IsEmpty() {
		return ""
	}
	val := s.top.data
	s.top = s.top.next
	return val
}

// Проверка на пустоту
func (s *Stack) IsEmpty() bool {
	return s.top == nil
}

// Вывод элементов стека
func (s *Stack) Print() {
	curr := s.top
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

// Сохранение в файл
func (s *Stack) SaveToFile(filename string) {
	file, err := os.Create(filename)
	if err != nil {
		return
	}
	defer file.Close()

	curr := s.top
	for curr != nil {
		file.WriteString(curr.data + "\n")
		curr = curr.next
	}
}

// Загрузка из файла
func (s *Stack) LoadFromFile(filename string) {
	file, err := os.Open(filename)
	if err != nil {
		return
	}
	defer file.Close()

	s.top = nil

	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line != "" {
			lines = append(lines, line)
		}
	}
	if err := scanner.Err(); err != nil {
	}
	// Загружаем в обратном порядке, чтобы восстановить исходный стек
	for i := len(lines) - 1; i >= 0; i-- {
		s.Push(lines[i])
	}
}
