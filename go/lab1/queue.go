package lab1

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type QNode struct {
	Data string
	Next *QNode
}
type Queue struct{ head, tail *QNode }

func NewQueue() *Queue { return &Queue{} }
func (q *Queue) Push(value string) {
	node := &QNode{Data: value}
	if q.tail == nil {
		q.head, q.tail = node, node
		return
	}
	q.tail.Next = node
	q.tail = node
}
func (q *Queue) Pop() string {
	if q.head == nil {
		return ""
	}
	value := q.head.Data
	q.head = q.head.Next
	if q.head == nil {
		q.tail = nil
	}
	return value
}
func (q *Queue) IsEmpty() bool { return q.head == nil }
func (q *Queue) Print() {
	for node := q.head; node != nil; node = node.Next {
		fmt.Print(node.Data, " ")
	}
	fmt.Println()
}
func (q *Queue) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	for node := q.head; node != nil; node = node.Next {
		if _, err := fmt.Fprintln(file, node.Data); err != nil {
			return err
		}
	}
	return nil
}
func (q *Queue) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	q.head, q.tail = nil, nil
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		value := strings.TrimSpace(scanner.Text())
		if value != "" {
			q.Push(value)
		}
	}
	return scanner.Err()
}
