TARGET = bin/kq
DATABASE = kata.db
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

init: clean default
	./$(TARGET) -f $(DATABASE) -n
	./$(TARGET) -f $(DATABASE) -a "implement a min-heap"
	./$(TARGET) -f $(DATABASE) -a "implement an arraylist"
	./$(TARGET) -f $(DATABASE) -a "implement a singly linked list"
	./$(TARGET) -f $(DATABASE) -a "implement a doubly linked list"
	./$(TARGET) -f $(DATABASE) -a "implement a hashmap"
	./$(TARGET) -f $(DATABASE) -a "implement linear search over an array"
	./$(TARGET) -f $(DATABASE) -a "implement binary search over an array, recursively"
	./$(TARGET) -f $(DATABASE) -a "implement binary search over a tree, iteratively"
	./$(TARGET) -f $(DATABASE) -a "implement dfs over a tree, iteratively"
	./$(TARGET) -f $(DATABASE) -a "implement dfs over a graph, iteratively"
	./$(TARGET) -f $(DATABASE) -a "implement dfs over a tree, recursively"
	./$(TARGET) -f $(DATABASE) -a "implement dfs over a graph, recursively"
	./$(TARGET) -f $(DATABASE) -a "implement bfs over a tree"
	./$(TARGET) -f $(DATABASE) -a "implement bfs over a graph"
	./$(TARGET) -f $(DATABASE) -g
	./$(TARGET) -f $(DATABASE) -d

test: clean default
	./$(TARGET) -f $(DATABASE) -n
	./$(TARGET) -f $(DATABASE) -a "implement a min-heap"
	./$(TARGET) -f $(DATABASE) -a "implement an arraylist"
	./$(TARGET) -f $(DATABASE) -a "implement a hashmap"
	./$(TARGET) -f $(DATABASE) -a "oops: remove me"
	./$(TARGET) -f $(DATABASE) -a "oops: remove me 2"
	./$(TARGET) -f $(DATABASE) -a "implement linear search over an array"
	./$(TARGET) -f $(DATABASE) -r "oops: remove me"
	./$(TARGET) -f $(DATABASE) -r "oops: remove me 2"
	./$(TARGET) -f $(DATABASE) -a "implement binary search over an array"
	./$(TARGET) -f $(DATABASE) -k
	./$(TARGET) -f $(DATABASE) -g
	./$(TARGET) -f $(DATABASE) -q
	./$(TARGET) -f $(DATABASE) -d
	./$(TARGET) -f $(DATABASE) -q
	./$(TARGET) -f $(DATABASE) -d
	./$(TARGET) -f $(DATABASE) -q
	./$(TARGET) -f $(DATABASE) -e
	./$(TARGET) -f $(DATABASE) -q
	./$(TARGET) -f $(DATABASE) -w
	./$(TARGET) -f $(DATABASE) -k


build: clean default

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude

