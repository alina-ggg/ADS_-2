package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
)

// IntegerSet удаляет повторы, поскольку входные данные рассматриваются
// как множество натуральных чисел.
type IntegerSet struct {
	values map[int]struct{}
}

func newIntegerSet() *IntegerSet {
	return &IntegerSet{values: make(map[int]struct{})}
}

func (set *IntegerSet) Add(value int) bool {
	if _, exists := set.values[value]; exists {
		return false
	}

	set.values[value] = struct{}{}
	return true
}

// PartitionSolver ищет разбиение всех чисел на группы одинаковой суммы.
type PartitionSolver struct {
	numbers    []int
	used       []bool
	groups     [][]int
	targetSum  int
	groupCount int
}

func newPartitionSolver(numbers []int, target int) *PartitionSolver {
	copyOfNumbers := append([]int(nil), numbers...)
	return &PartitionSolver{
		numbers:   copyOfNumbers,
		used:      make([]bool, len(copyOfNumbers)),
		targetSum: target,
	}
}

// buildGroups строит очередную группу рекурсивным перебором комбинаций.
// Текущий список передаётся как аргумент, поэтому откат не изменяет
// состояние соседних ветвей поиска.
func (solver *PartitionSolver) buildGroups(
	groupIndex int,
	startIndex int,
	currentSum int,
	currentGroup []int,
) bool {
	if groupIndex == solver.groupCount-1 {
		lastGroup := make([]int, 0)
		lastSum := 0

		for index, value := range solver.numbers {
			if !solver.used[index] {
				lastGroup = append(lastGroup, value)
				lastSum += value
			}
		}

		if lastSum != solver.targetSum {
			return false
		}

		solver.groups = append(solver.groups, lastGroup)
		return true
	}

	if currentSum == solver.targetSum {
		completedGroup := append([]int(nil), currentGroup...)
		solver.groups = append(solver.groups, completedGroup)

		if solver.buildGroups(groupIndex+1, 0, 0, []int{}) {
			return true
		}

		solver.groups = solver.groups[:len(solver.groups)-1]
		return false
	}

	previousValue := -1

	for index := startIndex; index < len(solver.numbers); index++ {
		value := solver.numbers[index]

		if solver.used[index] {
			continue
		}
		if value == previousValue {
			continue
		}
		if currentSum+value > solver.targetSum {
			continue
		}

		previousValue = value
		solver.used[index] = true
		nextGroup := append(append([]int(nil), currentGroup...), value)

		if solver.buildGroups(groupIndex, index+1, currentSum+value, nextGroup) {
			return true
		}

		solver.used[index] = false

		// Если группа пока пуста и выбранное наименьшее число
		// не привело к решению, проверять другие первые элементы не нужно.
		if currentSum == 0 {
			return false
		}
	}

	return false
}

func (solver *PartitionSolver) Solve() bool {
	if len(solver.numbers) == 0 || solver.targetSum <= 0 {
		return false
	}

	totalSum := 0
	for _, value := range solver.numbers {
		totalSum += value
	}

	if totalSum == 0 || totalSum%solver.targetSum != 0 {
		return false
	}

	solver.groupCount = totalSum / solver.targetSum
	sort.Sort(sort.Reverse(sort.IntSlice(solver.numbers)))

	if solver.numbers[0] > solver.targetSum {
		return false
	}

	return solver.buildGroups(0, 0, 0, []int{})
}

func (solver *PartitionSolver) PrintAnswer() {
	for groupIndex, group := range solver.groups {
		if groupIndex > 0 {
			fmt.Print(" | ")
		}

		fmt.Print("{")
		for elementIndex, value := range group {
			if elementIndex > 0 {
				fmt.Print(", ")
			}
			fmt.Print(value)
		}
		fmt.Print("}")
	}
	fmt.Println()
}

func main() {
	input := bufio.NewReader(os.Stdin)

	var count int
	if _, err := fmt.Fscan(input, &count); err != nil || count <= 0 || count > 30 {
		fmt.Fprintln(os.Stderr, "Количество элементов должно быть от 1 до 30.")
		return
	}

	uniqueNumbers := newIntegerSet()
	numbers := make([]int, 0, count)

	for index := 0; index < count; index++ {
		var value int
		if _, err := fmt.Fscan(input, &value); err != nil || value <= 0 {
			fmt.Fprintln(os.Stderr, "Все элементы должны быть натуральными числами.")
			return
		}

		if uniqueNumbers.Add(value) {
			numbers = append(numbers, value)
		}
	}

	var target int
	if _, err := fmt.Fscan(input, &target); err != nil {
		fmt.Fprintln(os.Stderr, "Не задана целевая сумма группы.")
		return
	}

	solver := newPartitionSolver(numbers, target)
	if !solver.Solve() {
		fmt.Println("Impossible")
		return
	}

	solver.PrintAnswer()
}
