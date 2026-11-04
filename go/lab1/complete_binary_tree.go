package lab1

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type CBTNode struct {
	Data        int
	Left, Right *CBTNode
}
type CompleteBinaryTree struct{ root *CBTNode }

func NewCompleteBinaryTree() *CompleteBinaryTree { return &CompleteBinaryTree{} }
func (tree *CompleteBinaryTree) Insert(value int) {
	node := &CBTNode{Data: value}
	if tree.root == nil {
		tree.root = node
		return
	}
	queue := []*CBTNode{tree.root}
	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]
		if current.Left == nil {
			current.Left = node
			return
		}
		queue = append(queue, current.Left)
		if current.Right == nil {
			current.Right = node
			return
		}
		queue = append(queue, current.Right)
	}
}
func (tree *CompleteBinaryTree) count(node *CBTNode) int {
	if node == nil {
		return 0
	}
	return 1 + tree.count(node.Left) + tree.count(node.Right)
}
func (tree *CompleteBinaryTree) complete(node *CBTNode, index, count int) bool {
	if node == nil {
		return true
	}
	if index >= count {
		return false
	}
	return tree.complete(node.Left, 2*index+1, count) && tree.complete(node.Right, 2*index+2, count)
}
func (tree *CompleteBinaryTree) IsComplete() bool {
	return tree.complete(tree.root, 0, tree.count(tree.root))
}
func (tree *CompleteBinaryTree) Search(value int) bool {
	if tree.root == nil {
		return false
	}
	queue := []*CBTNode{tree.root}
	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]
		if current.Data == value {
			return true
		}
		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}
	return false
}
func (tree *CompleteBinaryTree) Clear() { tree.root = nil }
func (tree *CompleteBinaryTree) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	if tree.root == nil {
		return nil
	}
	queue := []*CBTNode{tree.root}
	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]
		if _, err := fmt.Fprintln(file, current.Data); err != nil {
			return err
		}
		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}
	return nil
}
func (tree *CompleteBinaryTree) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	tree.Clear()
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		value, e := strconv.Atoi(strings.TrimSpace(scanner.Text()))
		if e == nil {
			tree.Insert(value)
		}
	}
	return scanner.Err()
}
func (tree *CompleteBinaryTree) PrintLevelOrder() {
	if tree.root == nil {
		fmt.Println("Tree is empty")
		return
	}
	queue := []*CBTNode{tree.root}
	fmt.Println("Tree structure (Level by level):")
	for len(queue) > 0 {
		size := len(queue)
		values := make([]string, 0, size)
		for i := 0; i < size; i++ {
			current := queue[0]
			queue = queue[1:]
			values = append(values, strconv.Itoa(current.Data))
			if current.Left != nil {
				queue = append(queue, current.Left)
			}
			if current.Right != nil {
				queue = append(queue, current.Right)
			}
		}
		fmt.Println(strings.Join(values, " "))
	}
}
