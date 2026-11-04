CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic
LAB1 = lab1

all: task1 task2 task3 task4 task5 task6 task7

task1: task1.cpp $(LAB1)/stack.cpp $(LAB1)/stack.h
	$(CXX) $(CXXFLAGS) task1.cpp $(LAB1)/stack.cpp -o $@

task2: task2.cpp
	$(CXX) $(CXXFLAGS) task2.cpp -o $@

task3: task3.cpp
	$(CXX) $(CXXFLAGS) task3.cpp -o $@

task4: task4.cpp $(LAB1)/array.cpp $(LAB1)/array.h
	$(CXX) $(CXXFLAGS) task4.cpp $(LAB1)/array.cpp -o $@

task5: task5.cpp
	$(CXX) $(CXXFLAGS) task5.cpp -o $@

task6: task6.cpp
	$(CXX) $(CXXFLAGS) task6.cpp -o $@

task7: task7.cpp $(LAB1)/doubly_linked_list.cpp $(LAB1)/doubly_linked_list.h
	$(CXX) $(CXXFLAGS) task7.cpp $(LAB1)/doubly_linked_list.cpp -o $@

clean:
	rm -f task1 task2 task3 task4 task5 task6 task7

.PHONY: all clean
