package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
)

type IntegerSet struct{ values map[int]struct{} }

func newIntegerSet() *IntegerSet { return &IntegerSet{values: make(map[int]struct{})} }
func (set *IntegerSet) Add(value int) bool {
	if _, exists := set.values[value]; exists {
		return false
	}
	set.values[value] = struct{}{}
	return true
}

type SetPartitionSolver struct {
	numbers       []int
	originalOrder []int
	used          []bool
	answer        [][]int
	targetSum     int
	groupCount    int
}

func newSetPartitionSolver(input []int, target int) *SetPartitionSolver {
	numbers := append([]int(nil), input...)
	return &SetPartitionSolver{
		numbers:       numbers,
		originalOrder: append([]int(nil), input...),
		used:          make([]bool, len(input)),
		targetSum:     target,
	}
}

func (solver *SetPartitionSolver) buildGroups(groupIndex, startIndex, currentSum int, currentGroup []int) bool {
	if groupIndex == solver.groupCount-1 {
		lastGroup := make([]int, 0)
		lastSum := 0
		for i, value := range solver.numbers {
			if !solver.used[i] {
				lastGroup = append(lastGroup, value)
				lastSum += value
			}
		}
		if lastSum != solver.targetSum {
			return false
		}
		solver.answer = append(solver.answer, lastGroup)
		return true
	}

	if currentSum == solver.targetSum {
		solver.answer = append(solver.answer, append([]int(nil), currentGroup...))
		if solver.buildGroups(groupIndex+1, 0, 0, []int{}) {
			return true
		}
		solver.answer = solver.answer[:len(solver.answer)-1]
		return false
	}

	previousValue := -1
	for i := startIndex; i < len(solver.numbers); i++ {
		value := solver.numbers[i]
		if solver.used[i] || value == previousValue || currentSum+value > solver.targetSum {
			continue
		}
		previousValue = value
		solver.used[i] = true
		nextGroup := append(append([]int(nil), currentGroup...), value)
		if solver.buildGroups(groupIndex, i+1, currentSum+value, nextGroup) {
			return true
		}
		solver.used[i] = false
	}
	return false
}

func (solver *SetPartitionSolver) Solve() bool {
	if len(solver.numbers) == 0 || solver.targetSum <= 0 {
		return false
	}
	total := 0
	for _, value := range solver.numbers {
		total += value
	}
	if total%solver.targetSum != 0 {
		return false
	}
	solver.groupCount = total / solver.targetSum

	hasSingleElement := false
	for _, value := range solver.numbers {
		if value == solver.targetSum {
			hasSingleElement = true
			break
		}
	}
	if hasSingleElement {
		sort.Sort(sort.Reverse(sort.IntSlice(solver.numbers)))
	}
	if len(solver.numbers) == 0 || solver.numbers[0] > solver.targetSum {
		return false
	}
	if !solver.buildGroups(0, 0, 0, []int{}) {
		return false
	}

	// Восстанавливаем исходный порядок элементов внутри каждой группы.
	position := make(map[int]int, len(solver.originalOrder))
	for i, value := range solver.originalOrder {
		position[value] = i
	}
	for _, group := range solver.answer {
		sort.Slice(group, func(i, j int) bool { return position[group[i]] < position[group[j]] })
	}
	return true
}

func main() {
	input := bufio.NewReader(os.Stdin)
	var count int
	if _, err := fmt.Fscan(input, &count); err != nil || count <= 0 || count > 30 {
		fmt.Fprintln(os.Stderr, "Количество элементов должно быть от 1 до 30.")
		os.Exit(1)
	}
	unique := newIntegerSet()
	numbers := make([]int, 0, count)
	for i := 0; i < count; i++ {
		var value int
		if _, err := fmt.Fscan(input, &value); err != nil || value <= 0 {
			fmt.Fprintln(os.Stderr, "Все элементы должны быть натуральными числами.")
			os.Exit(1)
		}
		if unique.Add(value) {
			numbers = append(numbers, value)
		}
	}
	var target int
	if _, err := fmt.Fscan(input, &target); err != nil {
		fmt.Fprintln(os.Stderr, "Не задана целевая сумма группы.")
		os.Exit(1)
	}
	solver := newSetPartitionSolver(numbers, target)
	if !solver.Solve() {
		fmt.Println("Impossible")
		return
	}
	for i, group := range solver.answer {
		if i > 0 {
			fmt.Print(" | ")
		}
		fmt.Print("{")
		for j, value := range group {
			if j > 0 {
				fmt.Print(", ")
			}
			fmt.Print(value)
		}
		fmt.Print("}")
	}
	fmt.Println()
}
