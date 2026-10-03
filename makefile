CXX = g++
CXXFLAGS = -std=c++17 -Wall -I.

# все исходники проекта
SRC = main.cpp \
      structures/array.cpp \
      structures/forward_list.cpp \
      structures/doubly_list.cpp \
      structures/stack.cpp \
      structures/queue.cpp \
      structures/deque.cpp \
      structures/avl_tree.cpp \
      structures/registry.cpp \
      utils/flags.cpp \
      utils/json_io.cpp \
      utils/file_utils.cpp

# куда собираем бинарник
TARGET = bin/dbms

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	@mkdir -p data
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET) --file data.json --query 'PRINT'

clean:
	rm -rf bin/* data/*.json

.PHONY: all run clean