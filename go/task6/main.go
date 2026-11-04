package main

import (
	"bufio"
	"fmt"
	"hash/fnv"
	"os"
	"time"
)

// HashTable — таблица с открытой адресацией.
// В зависимости от параметра doubleHashing используется линейное
// пробирование или двойное хеширование.
type HashTable struct {
	keys          []string
	occupied      []bool
	doubleHashing bool
	collisions    int
}

func hashString(value string) uint64 {
	hasher := fnv.New64a()
	_, _ = hasher.Write([]byte(value))
	return hasher.Sum64()
}

func isPrime(number int) bool {
	if number < 2 {
		return false
	}

	for divisor := 2; divisor*divisor <= number; divisor++ {
		if number%divisor == 0 {
			return false
		}
	}

	return true
}

func nextPrime(number int) int {
	for !isPrime(number) {
		number++
	}
	return number
}

func NewHashTable(expectedElements int, useDoubleHashing bool) *HashTable {
	if expectedElements < 0 {
		expectedElements = 0
	}

	tableSize := nextPrime(expectedElements*2 + 3)

	return &HashTable{
		keys:          make([]string, tableSize),
		occupied:      make([]bool, tableSize),
		doubleHashing: useDoubleHashing,
		collisions:    0,
	}
}

func (table *HashTable) primaryHash(value string) int {
	return int(hashString(value) % uint64(len(table.keys)))
}

func (table *HashTable) secondaryHash(value string) int {
	// Таблица имеет простой размер, а шаг находится в диапазоне
	// от 1 до размера-1. Следовательно, пробирование сможет пройти
	// по всем ячейкам таблицы.
	secondHash := hashString(value + "#step")
	return 1 + int(secondHash%uint64(len(table.keys)-1))
}

func (table *HashTable) probeIndex(value string, attempt int) int {
	start := table.primaryHash(value)
	step := 1

	if table.doubleHashing {
		step = table.secondaryHash(value)
	}

	return (start + attempt*step) % len(table.keys)
}

// Insert добавляет ключ. Повторный ключ не создаёт новую запись.
func (table *HashTable) Insert(value string) bool {
	for attempt := 0; attempt < len(table.keys); attempt++ {
		index := table.probeIndex(value, attempt)

		if !table.occupied[index] {
			table.occupied[index] = true
			table.keys[index] = value
			return true
		}

		if table.keys[index] == value {
			return false
		}

		table.collisions++
	}

	return false
}

// Contains выполняет поиск ключа тем же способом пробирования,
// который использовался при его вставке.
func (table *HashTable) Contains(value string) bool {
	for attempt := 0; attempt < len(table.keys); attempt++ {
		index := table.probeIndex(value, attempt)

		if !table.occupied[index] {
			return false
		}

		if table.keys[index] == value {
			return true
		}
	}

	return false
}

func (table *HashTable) CollisionCount() int {
	return table.collisions
}

func (table *HashTable) Capacity() int {
	return len(table.keys)
}

type ExperimentResult struct {
	found       bool
	collisions  int
	elapsedTime time.Duration
	capacity    int
}

func runExperiment(values []string, searchValue string, doubleHashing bool) ExperimentResult {
	table := NewHashTable(len(values)+1, doubleHashing)

	startTime := time.Now()
	for _, value := range values {
		table.Insert(value)
	}
	elapsed := time.Since(startTime)

	return ExperimentResult{
		found:       table.Contains(searchValue),
		collisions:  table.CollisionCount(),
		elapsedTime: elapsed,
		capacity:    table.Capacity(),
	}
}

func printExperiment(name string, result ExperimentResult) {
	fmt.Printf(
		"%s: found=%t, collisions=%d, capacity=%d, insertion_time=%d us\n",
		name,
		result.found,
		result.collisions,
		result.capacity,
		result.elapsedTime.Microseconds(),
	)
}

func runHashTableComparison() {
	input := bufio.NewReader(os.Stdin)

	var count int
	if _, err := fmt.Fscan(input, &count); err != nil || count < 0 || count > 100000 {
		fmt.Fprintln(os.Stderr, "Некорректное количество ключей.")
		return
	}

	values := make([]string, count)
	for index := 0; index < count; index++ {
		if _, err := fmt.Fscan(input, &values[index]); err != nil {
			fmt.Fprintln(os.Stderr, "Не удалось прочитать ключ.")
			return
		}
	}

	var searchValue string
	if _, err := fmt.Fscan(input, &searchValue); err != nil {
		fmt.Fprintln(os.Stderr, "Не задан ключ для поиска.")
		return
	}

	linearResult := runExperiment(values, searchValue, false)
	doubleResult := runExperiment(values, searchValue, true)

	printExperiment("Linear probing", linearResult)
	printExperiment("Double hashing", doubleResult)
}

// longestUniqueSubstring применяет алгоритм скользящего окна.
// В map хранится последняя позиция каждого символа строки.
func longestUniqueSubstring(value string) int {
	symbols := []rune(value)
	lastPosition := make(map[rune]int)

	leftBorder := 0
	maximumLength := 0

	for rightBorder, currentSymbol := range symbols {
		previousPosition, exists := lastPosition[currentSymbol]
		if exists && previousPosition >= leftBorder {
			leftBorder = previousPosition + 1
		}

		lastPosition[currentSymbol] = rightBorder

		currentLength := rightBorder - leftBorder + 1
		if currentLength > maximumLength {
			maximumLength = currentLength
		}
	}

	return maximumLength
}

func runLongestUniqueSubstring() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Buffer(make([]byte, 1024), 1024*1024)

	if !scanner.Scan() {
		fmt.Fprintln(os.Stderr, "Не удалось прочитать строку.")
		return
	}

	input := scanner.Text()
	fmt.Println(longestUniqueSubstring(input))

	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка чтения строки:", err)
	}
}

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintln(os.Stderr, "Использование: go run . 6.1 | go run . 6.2")
		return
	}

	switch os.Args[1] {
	case "6.1":
		runHashTableComparison()
	case "6.2":
		runLongestUniqueSubstring()
	default:
		fmt.Fprintln(os.Stderr, "Неизвестное задание. Укажите 6.1 или 6.2.")
	}
}
