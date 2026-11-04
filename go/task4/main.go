package main

import (
	"bufio"
	"fmt"
	"os"
)

// SubsetGenerator хранит исходный массив из лабораторной №1
// и текущую комбинацию, которая формируется рекурсивно.
type SubsetGenerator struct {
	source         *Strarray
	currentSubset  []string
	generatedCount int
}

func NewSubsetGenerator(source *Strarray) *SubsetGenerator {
	return &SubsetGenerator{
		source:         source,
		currentSubset:  make([]string, 0),
		generatedCount: 0,
	}
}

func (generator *SubsetGenerator) printCurrentSubset() {
	fmt.Print("{")

	for index, value := range generator.currentSubset {
		if index > 0 {
			fmt.Print(", ")
		}
		fmt.Print(value)
	}

	fmt.Println("}")
	generator.generatedCount++
}

// generateOfSize выводит все подмножества заданного размера.
// Параметр startIndex не позволяет повторно выбирать уже использованные
// позиции и тем самым исключает перестановки одинакового подмножества.
func (generator *SubsetGenerator) generateOfSize(startIndex int, requiredSize int) {
	if len(generator.currentSubset) == requiredSize {
		generator.printCurrentSubset()
		return
	}

	remaining := requiredSize - len(generator.currentSubset)
	lastPossibleIndex := generator.source.MSIZE() - remaining

	for index := startIndex; index <= lastPossibleIndex; index++ {
		value := generator.source.ValueAt(index)
		generator.currentSubset = append(generator.currentSubset, value)

		generator.generateOfSize(index+1, requiredSize)

		// Возвращаем длину текущего набора к значению до рекурсивного вызова.
		generator.currentSubset = generator.currentSubset[:len(generator.currentSubset)-1]
	}
}

func (generator *SubsetGenerator) GenerateAll() {
	for subsetSize := 0; subsetSize <= generator.source.MSIZE(); subsetSize++ {
		generator.generateOfSize(0, subsetSize)
	}
}

func main() {
	input := bufio.NewReader(os.Stdin)

	var count int
	if _, err := fmt.Fscan(input, &count); err != nil || count < 0 || count > 25 {
		fmt.Fprintln(os.Stderr, "Количество элементов должно быть от 0 до 25.")
		return
	}

	array := NewStrarray()

	for index := 0; index < count; index++ {
		var value string
		if _, err := fmt.Fscan(input, &value); err != nil {
			fmt.Fprintln(os.Stderr, "Не удалось прочитать элемент массива.")
			return
		}

		array.MPUSH_end(value)
	}

	generator := NewSubsetGenerator(array)
	generator.GenerateAll()
}
