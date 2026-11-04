package lab1

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type StackNode struct {
	Data string
	Next *StackNode
}

type Stack struct {
	top *StackNode
}

func NewStack() *Stack { return &Stack{} }

func (s *Stack) Push(value string) {
	s.top = &StackNode{Data: value, Next: s.top}
}

func (s *Stack) Pop() string {
	if s.IsEmpty() {
		return ""
	}
	value := s.top.Data
	s.top = s.top.Next
	return value
}

func (s *Stack) IsEmpty() bool { return s.top == nil }

func (s *Stack) Print() {
	for node := s.top; node != nil; node = node.Next {
		fmt.Print(node.Data, " ")
	}
	fmt.Println()
}

func (s *Stack) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	for node := s.top; node != nil; node = node.Next {
		if _, err := file.WriteString(node.Data + "\n"); err != nil {
			return err
		}
	}
	return nil
}

func (s *Stack) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line != "" {
			lines = append(lines, line)
		}
	}
	if err := scanner.Err(); err != nil {
		return err
	}
	s.top = nil
	for i := len(lines) - 1; i >= 0; i-- {
		s.Push(lines[i])
	}
	return nil
}
