package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"

	"lab2/lab1"
)

// LRU-кэш использует узел DNode и двусвязный список из лаб №1
// Голова списка — самый недавно использованный элемент, хвост — самый старый
type LRUCache struct {
	capacity  int
	head      *lab1.DNode
	tail      *lab1.DNode
	nodeByKey map[int]*lab1.DNode
}

func NewLRUCache(capacity int) *LRUCache {
	return &LRUCache{
		capacity:  capacity,
		nodeByKey: make(map[int]*lab1.DNode),
	}
}

func makeNodeData(key, value int) string { return strconv.Itoa(key) + ":" + strconv.Itoa(value) }

func extractKey(node *lab1.DNode) int {
	separator := 0
	for separator < len(node.Data) && node.Data[separator] != ':' {
		separator++
	}
	key, _ := strconv.Atoi(node.Data[:separator])
	return key
}
func extractValue(node *lab1.DNode) int {
	separator := 0
	for separator < len(node.Data) && node.Data[separator] != ':' {
		separator++
	}
	value, _ := strconv.Atoi(node.Data[separator+1:])
	return value
}

func (cache *LRUCache) detachNode(node *lab1.DNode) {
	if node.Prev != nil {
		node.Prev.Next = node.Next
	} else {
		cache.head = node.Next
	}
	if node.Next != nil {
		node.Next.Prev = node.Prev
	} else {
		cache.tail = node.Prev
	}
	node.Prev = nil
	node.Next = nil
}

func (cache *LRUCache) moveToFront(node *lab1.DNode) {
	if node == cache.head {
		return
	}
	cache.detachNode(node)
	node.Next = cache.head
	if cache.head != nil {
		cache.head.Prev = node
	} else {
		cache.tail = node
	}
	cache.head = node
}

func (cache *LRUCache) removeLeastRecentlyUsed() {
	if cache.tail == nil {
		return
	}
	oldest := cache.tail
	delete(cache.nodeByKey, extractKey(oldest))
	cache.detachNode(oldest)
}

// Get возвращает значение по ключу или -1 и обновляет порядок использования
func (cache *LRUCache) Get(key int) int {
	node, exists := cache.nodeByKey[key]
	if !exists {
		return -1
	}
	cache.moveToFront(node)
	return extractValue(node)
}

// Set добавляет пару или обновляет существующую При переполнении
// удаляется пара, к которой дольше всего не обращались
func (cache *LRUCache) Set(key, value int) {
	if cache.capacity <= 0 {
		return
	}
	if node, exists := cache.nodeByKey[key]; exists {
		node.Data = makeNodeData(key, value)
		cache.moveToFront(node)
		return
	}
	if len(cache.nodeByKey) >= cache.capacity {
		cache.removeLeastRecentlyUsed()
	}
	node := lab1.LPUSHHEAD(&cache.head, &cache.tail, makeNodeData(key, value))
	cache.nodeByKey[key] = node
}

func (cache *LRUCache) Size() int { return len(cache.nodeByKey) }

func main() {
	input := bufio.NewReader(os.Stdin)
	var capacity, commandCount int
	if _, err := fmt.Fscan(input, &capacity, &commandCount); err != nil || capacity < 0 || commandCount < 0 {
		fmt.Fprintln(os.Stderr, "Некорректные параметры LRU-кэша.")
		os.Exit(1)
	}
	cache := NewLRUCache(capacity)
	results := make([]int, 0)

	for commandIndex := 0; commandIndex < commandCount; commandIndex++ {
		var command string
		var key int
		if _, err := fmt.Fscan(input, &command, &key); err != nil {
			fmt.Fprintln(os.Stderr, "Не удалось прочитать команду кэша.")
			os.Exit(1)
		}
		switch command {
		case "SET":
			var value int
			if _, err := fmt.Fscan(input, &value); err != nil {
				fmt.Fprintln(os.Stderr, "Для SET необходимо указать значение.")
				os.Exit(1)
			}
			cache.Set(key, value)
		case "GET":
			results = append(results, cache.Get(key))
		default:
			fmt.Fprintln(os.Stderr, "Неизвестная команда кэша:", command)
			os.Exit(1)
		}
	}

	writer := bufio.NewWriter(os.Stdout)
	for i, value := range results {
		if i > 0 {
			fmt.Fprint(writer, " ")
		}
		fmt.Fprint(writer, value)
	}
	fmt.Fprintln(writer)
	writer.Flush()
}
