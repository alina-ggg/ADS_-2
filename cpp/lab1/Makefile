# Компилятор
CXX = g++

# Флаги компиляции (стандарт C++17 и базовые предупреждения)
CXXFLAGS = -std=c++17 -Wall -Wextra

# Имя итогового исполняемого файла
TARGET = program

# Список всех исходных файлов проекта
SRCS = main.cpp array.cpp singly_linked_list.cpp doubly_linked_list.cpp stack.cpp queue.cpp complete_binary_tree.cpp

# Автоматическое преобразование .cpp в объектные файлы .o
OBJS = $(SRCS:.cpp=.o)

# Основная команда по умолчанию ('make')
all: $(TARGET)

# Сборка бинарника из объектных файлов
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Правило компиляции каждого отдельного файла
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка скомпилированных файлов ('make clean')
clean:
	rm -f $(OBJS) $(TARGET)