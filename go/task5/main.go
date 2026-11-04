package main

import (
	"bufio"
	"fmt"
	"os"
)

type TreeNode struct {
	key         int
	left, right *TreeNode
}
type BinarySearchTree struct{ root *TreeNode }

func NewBinarySearchTree() *BinarySearchTree { return &BinarySearchTree{} }
func (tree *BinarySearchTree) Insert(value int) {
	if tree.root == nil {
		tree.root = &TreeNode{key: value}
		return
	}
	current := tree.root
	for {
		if value < current.key {
			if current.left == nil {
				current.left = &TreeNode{key: value}
				return
			}
			current = current.left
		} else {
			if current.right == nil {
				current.right = &TreeNode{key: value}
				return
			}
			current = current.right
		}
	}
}
func (tree *BinarySearchTree) Height() int {
	if tree.root == nil {
		return 0
	}
	queue := []*TreeNode{tree.root}
	height := 0
	for len(queue) > 0 {
		levelSize := len(queue)
		height++
		for i := 0; i < levelSize; i++ {
			current := queue[0]
			queue = queue[1:]
			if current.left != nil {
				queue = append(queue, current.left)
			}
			if current.right != nil {
				queue = append(queue, current.right)
			}
		}
	}
	return height
}
func main() {
	input := bufio.NewReader(os.Stdin)
	tree := NewBinarySearchTree()
	for {
		var value int
		if _, err := fmt.Fscan(input, &value); err != nil {
			break
		}
		if value == 0 {
			break
		}
		tree.Insert(value)
	}
	fmt.Println(tree.Height())
}
