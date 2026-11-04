package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// LRUCache использует двусвязный список из лабораторной №1
// и map для быстрого доступа к узлу по ключу.
type LRUCache struct {
	capacity int
	head     *DNode
	tail     *DNode
	index    map[int]*DNode
}

func NewLRUCache(capacity int) *LRUCache {
	return &LRUCache{
		capacity: capacity,
		head:     nil,
		tail:     nil,
		index:    make(map[int]*DNode),
	}
}

func encodePair(key int, value int) string {
	return strconv.Itoa(key) + ":" + strconv.Itoa(value)
}

func decodeKey(node *DNode) int {
	separator := strings.IndexByte(node.data, ':')
	key, _ := strconv.Atoi(node.data[:separator])
	return key
}

func decodeValue(node *DNode) int {
	separator := strings.IndexByte(node.data, ':')
	value, _ := strconv.Atoi(node.data[separator+1:])
	return value
}

// detachNode удаляет узел из текущей позиции, не освобождая память.
func (cache *LRUCache) detachNode(node *DNode) {
	if node.prev != nil {
		node.prev.next = node.next
	} else {
		cache.head = node.next
	}

	if node.next != nil {
		node.next.prev = node.prev
	} else {
		cache.tail = node.prev
	}

	node.prev = nil
	node.next = nil
}

// moveToFront перемещает узел в голову списка.
// Голова соответствует наиболее недавно использованному элементу.
func (cache *LRUCache) moveToFront(node *DNode) {
	if cache.head == node {
		return
	}

	if node.prev != nil || node.next != nil || cache.tail == node {
		cache.detachNode(node)
	}

	node.prev = nil
	node.next = cache.head

	if cache.head != nil {
		cache.head.prev = node
	} else {
		cache.tail = node
	}

	cache.head = node
}

func (cache *LRUCache) removeLeastRecentlyUsed() {
	if cache.tail == nil {
		return
	}

	oldNode := cache.tail
	oldKey := decodeKey(oldNode)

	cache.detachNode(oldNode)
	delete(cache.index, oldKey)
}

// Get возвращает значение по ключу и обновляет порядок использования.
// Если ключ отсутствует, возвращается -1.
func (cache *LRUCache) Get(key int) int {
	node, exists := cache.index[key]
	if !exists {
		return -1
	}

	cache.moveToFront(node)
	return decodeValue(node)
}

// Set добавляет новую пару или обновляет существующую.
// При переполнении удаляется элемент из хвоста списка.
func (cache *LRUCache) Set(key int, value int) {
	if cache.capacity <= 0 {
		return
	}

	if node, exists := cache.index[key]; exists {
		node.data = encodePair(key, value)
		cache.moveToFront(node)
		return
	}

	if len(cache.index) >= cache.capacity {
		cache.removeLeastRecentlyUsed()
	}

	newNode := &DNode{data: encodePair(key, value)}
	cache.moveToFront(newNode)
	cache.index[key] = newNode
}

func (cache *LRUCache) Size() int {
	return len(cache.index)
}

func (cache *LRUCache) Clear() {
	current := cache.head

	for current != nil {
		next := current.next
		current.prev = nil
		current.next = nil
		current = next
	}

	cache.head = nil
	cache.tail = nil
	cache.index = make(map[int]*DNode)
}

func main() {
	input := bufio.NewReader(os.Stdin)

	var capacity int
	var commandCount int
	if _, err := fmt.Fscan(input, &capacity, &commandCount); err != nil ||
		capacity < 0 || commandCount < 0 {
		fmt.Fprintln(os.Stderr, "Некорректные параметры LRU-кэша.")
		return
	}

	cache := NewLRUCache(capacity)
	results := make([]string, 0)

	for commandIndex := 0; commandIndex < commandCount; commandIndex++ {
		var command string
		var key int

		if _, err := fmt.Fscan(input, &command, &key); err != nil {
			fmt.Fprintln(os.Stderr, "Не удалось прочитать команду кэша.")
			return
		}

		switch command {
		case "SET":
			var value int
			if _, err := fmt.Fscan(input, &value); err != nil {
				fmt.Fprintln(os.Stderr, "Для SET необходимо указать значение.")
				return
			}
			cache.Set(key, value)

		case "GET":
			results = append(results, strconv.Itoa(cache.Get(key)))

		default:
			fmt.Fprintln(os.Stderr, "Неизвестная команда кэша:", command)
			return
		}
	}

	output := bufio.NewWriter(os.Stdout)
	fmt.Fprintln(output, strings.Join(results, " "))
	output.Flush()
}
