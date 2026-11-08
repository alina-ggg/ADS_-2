CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic

LAB1 = lab1

LAB1_ARRAY = $(LAB1)/array.cpp
LAB1_STACK = $(LAB1)/stack.cpp
LAB1_SLL   = $(LAB1)/singly_linked_list.cpp

LAB1_ARRAY_H = $(LAB1)/array.h
LAB1_STACK_H = $(LAB1)/stack.h
LAB1_SLL_H   = $(LAB1)/singly_linked_list.h

HASH_TABLE_H = hash_table.h

all: task1 task2 task3 task4 task5 task6_1 task6_2 task7

task1: task1.cpp $(LAB1_ARRAY) $(LAB1_STACK) $(LAB1_ARRAY_H) $(LAB1_STACK_H)
	$(CXX) $(CXXFLAGS) task1.cpp $(LAB1_ARRAY) $(LAB1_STACK) -o $@

task2: task2.cpp $(LAB1_ARRAY) $(LAB1_SLL) $(LAB1_ARRAY_H) $(LAB1_SLL_H)
	$(CXX) $(CXXFLAGS) task2.cpp $(LAB1_ARRAY) $(LAB1_SLL) -o $@

task3: task3.cpp $(LAB1_ARRAY) $(LAB1_STACK) $(LAB1_ARRAY_H) $(LAB1_STACK_H)
	$(CXX) $(CXXFLAGS) task3.cpp $(LAB1_ARRAY) $(LAB1_STACK) -o $@

task4: task4.cpp $(LAB1_ARRAY) $(LAB1_STACK) $(LAB1_ARRAY_H) $(LAB1_STACK_H)
	$(CXX) $(CXXFLAGS) task4.cpp $(LAB1_ARRAY) $(LAB1_STACK) -o $@

task5: task5.cpp $(LAB1_ARRAY) $(LAB1_ARRAY_H)
	$(CXX) $(CXXFLAGS) task5.cpp $(LAB1_ARRAY) -o $@

task6_1: task6_1.cpp $(LAB1_ARRAY) $(LAB1_ARRAY_H)
	$(CXX) $(CXXFLAGS) task6_1.cpp $(LAB1_ARRAY) -o $@

task6_2: task6_2.cpp $(LAB1_ARRAY) $(LAB1_ARRAY_H) $(HASH_TABLE_H)
	$(CXX) $(CXXFLAGS) task6_2.cpp $(LAB1_ARRAY) -o $@

task7: task7.cpp $(LAB1_ARRAY) $(LAB1_SLL) $(LAB1_ARRAY_H) $(LAB1_SLL_H)
	$(CXX) $(CXXFLAGS) task7.cpp $(LAB1_ARRAY) $(LAB1_SLL) -o $@

clean:
	rm -f task1 task2 task3 task4 task5 task6_1 task6_2 task7

.PHONY: all clean