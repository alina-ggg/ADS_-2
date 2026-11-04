package main

import (
	"bufio"
	"fmt"
	"os"
)

type TreeNode struct {
	key   int
	left  *TreeNode
	right *TreeNode
}

func newTreeNode(value int) *TreeNode {
	return &TreeNode{
		key:   value,
		left:  nil,
		right: nil,
	}
}

// BinarySearchTree реализует обычное бинарное дерево поиска.
// Полное бинарное дерево из лабораторной №1 здесь не подходит,
// поскольку в BST положение узла определяется его ключом.
type BinarySearchTree struct {
	root *TreeNode
}

func NewBinarySearchTree() *BinarySearchTree {
	return &BinarySearchTree{root: nil}
}

func (tree *BinarySearchTree) Insert(value int) {
	if tree.root == nil {
		tree.root = newTreeNode(value)
		return
	}

	current := tree.root

	for {
		if value < current.key {
			if current.left == nil {
				current.left = newTreeNode(value)
				return
			}
			current = current.left
		} else {
			// Повторяющиеся ключи добавляются в правое поддерево.
			if current.right == nil {
				current.right = newTreeNode(value)
				return
			}
			current = current.right
		}
	}
}

// Height вычисляет число уровней дерева обходом в ширину.
// Для пустого дерева высота равна нулю.
func (tree *BinarySearchTree) Height() int {
	if tree.root == nil {
		return 0
	}

	queue := make([]*TreeNode, 0)
	queue = append(queue, tree.root)
	result := 0

	for len(queue) > 0 {
		nodesOnLevel := len(queue)
		result++

		for index := 0; index < nodesOnLevel; index++ {
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

	return result
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
