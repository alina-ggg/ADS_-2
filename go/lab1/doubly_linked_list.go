package lab1

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type DNode struct {
	Data string
	Prev *DNode
	Next *DNode
}

func LPUSHHEAD(head **DNode, tail **DNode, value string) *DNode {
	node := &DNode{Data: value, Next: *head}
	if *head != nil {
		(*head).Prev = node
	} else {
		*tail = node
	}
	*head = node
	return node
}

func LPUSHTAIL(head **DNode, tail **DNode, value string) *DNode {
	node := &DNode{Data: value, Prev: *tail}
	if *tail != nil {
		(*tail).Next = node
	} else {
		*head = node
	}
	*tail = node
	return node
}

func LPUSHBEFORE(head **DNode, tail **DNode, target, value string) *DNode {
	for current := *head; current != nil; current = current.Next {
		if current.Data == target {
			node := &DNode{Data: value, Prev: current.Prev, Next: current}
			if current.Prev != nil {
				current.Prev.Next = node
			} else {
				*head = node
			}
			current.Prev = node
			return node
		}
	}
	return nil
}

func LPUSHAFTER(head **DNode, tail **DNode, target, value string) *DNode {
	for current := *head; current != nil; current = current.Next {
		if current.Data == target {
			node := &DNode{Data: value, Prev: current, Next: current.Next}
			if current.Next != nil {
				current.Next.Prev = node
			} else {
				*tail = node
			}
			current.Next = node
			return node
		}
	}
	return nil
}

func LDELHEAD(head **DNode, tail **DNode) *DNode {
	if *head == nil {
		return nil
	}
	removed := *head
	*head = removed.Next
	if *head != nil {
		(*head).Prev = nil
	} else {
		*tail = nil
	}
	removed.Next, removed.Prev = nil, nil
	return removed
}

func LDELTAIL(head **DNode, tail **DNode) *DNode {
	if *tail == nil {
		return nil
	}
	removed := *tail
	*tail = removed.Prev
	if *tail != nil {
		(*tail).Next = nil
	} else {
		*head = nil
	}
	removed.Next, removed.Prev = nil, nil
	return removed
}

func LDELVALUE(head **DNode, tail **DNode, value string) *DNode {
	for current := *head; current != nil; current = current.Next {
		if current.Data == value {
			if current.Prev != nil {
				current.Prev.Next = current.Next
			} else {
				*head = current.Next
			}
			if current.Next != nil {
				current.Next.Prev = current.Prev
			} else {
				*tail = current.Prev
			}
			current.Prev, current.Next = nil, nil
			return current
		}
	}
	return nil
}

func LDELBEFORE(head **DNode, tail **DNode, target string) *DNode {
	for current := *head; current != nil; current = current.Next {
		if current.Data == target && current.Prev != nil {
			removed := current.Prev
			if removed.Prev != nil {
				removed.Prev.Next = current
			} else {
				*head = current
			}
			current.Prev = removed.Prev
			removed.Prev, removed.Next = nil, nil
			return removed
		}
	}
	return nil
}

func LDELAFTER(head **DNode, tail **DNode, target string) *DNode {
	for current := *head; current != nil; current = current.Next {
		if current.Data == target && current.Next != nil {
			removed := current.Next
			current.Next = removed.Next
			if removed.Next != nil {
				removed.Next.Prev = current
			} else {
				*tail = current
			}
			removed.Prev, removed.Next = nil, nil
			return removed
		}
	}
	return nil
}

func LSEARCH(head *DNode, value string) *DNode {
	position := 0
	for current := head; current != nil; current = current.Next {
		if current.Data == value {
			fmt.Printf("Элемент найден на позиции: %d\n", position)
			return current
		}
		position++
	}
	fmt.Println("Элемент не найден.")
	return nil
}

func PrintList(head *DNode) {
	for current := head; current != nil; current = current.Next {
		fmt.Print(current.Data, " ")
	}
	fmt.Println()
}

func LSAVETOFILE(head *DNode, filename string) error {
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

func LLOAD(filename string) (*DNode, *DNode, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()
	var head, tail *DNode
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		value := strings.TrimSpace(scanner.Text())
		if value != "" {
			LPUSHTAIL(&head, &tail, value)
		}
	}
	return head, tail, scanner.Err()
}
