package lab1

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type SNode struct {
	Data string
	Next *SNode
}

func FPUSHHEAD(head *SNode, value string) *SNode { return &SNode{Data: value, Next: head} }

func FPUSHTAIL(head *SNode, value string) *SNode {
	node := &SNode{Data: value}
	if head == nil {
		return node
	}
	for current := head; ; current = current.Next {
		if current.Next == nil {
			current.Next = node
			break
		}
	}
	return head
}

func FPUSHBEFORE(head *SNode, target, value string) *SNode {
	if head == nil {
		return nil
	}
	if head.Data == target {
		return FPUSHHEAD(head, value)
	}
	for current := head; current.Next != nil; current = current.Next {
		if current.Next.Data == target {
			current.Next = &SNode{Data: value, Next: current.Next}
			break
		}
	}
	return head
}

func FPUSHAFTER(head *SNode, target, value string) *SNode {
	for current := head; current != nil; current = current.Next {
		if current.Data == target {
			current.Next = &SNode{Data: value, Next: current.Next}
			break
		}
	}
	return head
}

func FDELHEAD(head *SNode) *SNode {
	if head == nil {
		return nil
	}
	return head.Next
}

func FDELTAIL(head *SNode) *SNode {
	if head == nil || head.Next == nil {
		return nil
	}
	for current := head; current.Next != nil; current = current.Next {
		if current.Next.Next == nil {
			current.Next = nil
			break
		}
	}
	return head
}

func FDELVALUE(head *SNode, value string) *SNode {
	if head == nil {
		return nil
	}
	if head.Data == value {
		return head.Next
	}
	for current := head; current.Next != nil; current = current.Next {
		if current.Next.Data == value {
			current.Next = current.Next.Next
			break
		}
	}
	return head
}

func FSEARCH(head *SNode, value string) *SNode {
	position := 0
	for current := head; current != nil; current = current.Next {
		if current.Data == value {
			fmt.Printf("Элемент найден на позиции: %d\n", position)
			return current
		}
		position++
	}
	fmt.Println("Элемент не найден")
	return nil
}

func PrintSinglyList(head *SNode) {
	for current := head; current != nil; current = current.Next {
		fmt.Print(current.Data, " ")
	}
	fmt.Println()
}

func FSAVETOFILE(head *SNode, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	for current := head; current != nil; current = current.Next {
		if _, err := fmt.Fprintln(file, current.Data); err != nil {
			return err
		}
	}
	return nil
}

func FLOAD(filename string) (*SNode, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var head, tail *SNode
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		value := strings.TrimSpace(scanner.Text())
		if value == "" {
			continue
		}
		node := &SNode{Data: value}
		if head == nil {
			head, tail = node, node
		} else {
			tail.Next = node
			tail = node
		}
	}
	return head, scanner.Err()
}
