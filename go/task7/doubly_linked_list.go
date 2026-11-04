package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type DNode struct {
	data string
	prev *DNode
	next *DNode
}

// Добавление в голову
func LPUSHHEAD(head *DNode, value string) *DNode {
	newNode := &DNode{data: value, prev: nil, next: head}
	if head != nil {
		head.prev = newNode
	}
	return newNode
}

// Добавление в хвост
func LPUSHTAIL(head *DNode, value string) *DNode {
	newNode := &DNode{data: value, prev: nil, next: nil}
	if head == nil {
		return newNode
	}

	curr := head
	for curr.next != nil {
		curr = curr.next
	}
	curr.next = newNode
	newNode.prev = curr
	return head
}

// Добавление перед целевым значением
func LPUSHBEFORE(head *DNode, target string, value string) *DNode {
	if head == nil {
		return nil
	}
	if head.data == target {
		return LPUSHHEAD(head, value)
	}

	curr := head
	for curr != nil && curr.data != target {
		curr = curr.next
	}

	if curr == nil {
		return head // Цель не найдена
	}

	newNode := &DNode{data: value, prev: curr.prev, next: curr}
	if curr.prev != nil {
		curr.prev.next = newNode
	}
	curr.prev = newNode
	return head
}

// Добавление после целевого значения
func LPUSHAFTER(head *DNode, target string, value string) *DNode {
	curr := head
	for curr != nil && curr.data != target {
		curr = curr.next
	}

	if curr == nil {
		return head // Цель не найдена
	}

	newNode := &DNode{data: value, prev: curr, next: curr.next}
	if curr.next != nil {
		curr.next.prev = newNode
	}
	curr.next = newNode
	return head
}

// Удаление головы
func LDELHEAD(head *DNode) *DNode {
	if head == nil {
		return nil
	}
	head = head.next
	if head != nil {
		head.prev = nil
	}
	return head
}

// Удаление хвоста
func LDELTAIL(head *DNode) *DNode {
	if head == nil {
		return nil
	}
	if head.next == nil {
		return nil
	}

	curr := head
	for curr.next != nil {
		curr = curr.next
	}

	if curr.prev != nil {
		curr.prev.next = nil
	}
	return head
}

// Удаление по значению
func LDELVALUE(head *DNode, value string) *DNode {
	if head == nil {
		return nil
	}
	if head.data == value {
		return LDELHEAD(head)
	}

	curr := head
	for curr != nil && curr.data != value {
		curr = curr.next
	}

	if curr == nil {
		return head // Значение не найдено
	}

	if curr.next != nil {
		curr.next.prev = curr.prev
	}
	if curr.prev != nil {
		curr.prev.next = curr.next
	}
	return head
}

// Поиск элемента по значению
func LSEARCH(head *DNode, value string) *DNode {
	curr := head
	index := 0
	for curr != nil {
		if curr.data == value {
			fmt.Printf("Элемент найден на позиции: %d\n", index)
			return curr
		}
		curr = curr.next
		index++
	}
	fmt.Println("Элемент не найден.")
	return nil
}

// Вывод списка
func PrintList(head *DNode) {
	curr := head
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

// Сохранение в файл
func LSAVETOFILE(head *DNode, filename string) {
	file, err := os.Create(filename)
	if err != nil {
		return
	}
	defer file.Close()

	curr := head
	for curr != nil {
		file.WriteString(curr.data + "\n")
		curr = curr.next
	}
}

// Загрузка из файла
func LLOAD(filename string) *DNode {
	file, err := os.Open(filename)
	if err != nil {
		return nil
	}
	defer file.Close()

	var head *DNode
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line != "" {
			head = LPUSHTAIL(head, line)
		}
	}
	if err := scanner.Err(); err != nil {
	}
	return head
}
