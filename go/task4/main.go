package main

import (
	"bufio"
	"fmt"
	"os"

	"lab2/lab1"
)

type SubsetGenerator struct {
	source         *lab1.Strarray
	currentSubset  []string
	generatedCount int
}

func NewSubsetGenerator(source *lab1.Strarray) *SubsetGenerator {
	return &SubsetGenerator{source: source, currentSubset: make([]string, 0)}
}

func (generator *SubsetGenerator) printCurrentSubset() {
	fmt.Print("{")
	for i, value := range generator.currentSubset {
		if i > 0 {
			fmt.Print(", ")
		}
		fmt.Print(value)
	}
	fmt.Println("}")
	generator.generatedCount++
}

func (generator *SubsetGenerator) generateFrom(startIndex, requiredSize int) {
	if len(generator.currentSubset) == requiredSize {
		generator.printCurrentSubset()
		return
	}
	remaining := requiredSize - len(generator.currentSubset)
	lastPossibleIndex := generator.source.MSIZE() - remaining
	for index := startIndex; index <= lastPossibleIndex; index++ {
		generator.currentSubset = append(generator.currentSubset, generator.source.MVALUE_index(index))
		generator.generateFrom(index+1, requiredSize)
		generator.currentSubset = generator.currentSubset[:len(generator.currentSubset)-1]
	}
}

func (generator *SubsetGenerator) GenerateAll() {
	for size := 0; size <= generator.source.MSIZE(); size++ {
		generator.generateFrom(0, size)
	}
}

func main() {
	input := bufio.NewReader(os.Stdin)
	var count int
	if _, err := fmt.Fscan(input, &count); err != nil || count < 0 || count > 25 {
		fmt.Fprintln(os.Stderr, "Количество элементов должно быть от 0 до 25.")
		os.Exit(1)
	}
	array := lab1.NewStrarray()
	for i := 0; i < count; i++ {
		var value string
		if _, err := fmt.Fscan(input, &value); err != nil {
			fmt.Fprintln(os.Stderr, "Не удалось прочитать элемент массива.")
			os.Exit(1)
		}
		array.MPUSH_end(value)
	}
	NewSubsetGenerator(array).GenerateAll()
}
