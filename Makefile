TARGET = bin/kq
DATABASE = kata.db
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

init: clean default
	./$(TARGET) -f $(DATABASE) -n
	./$(TARGET) -f $(DATABASE) -a "data structure: min-heap"
	./$(TARGET) -f $(DATABASE) -a "data structure: max-heap"
	./$(TARGET) -f $(DATABASE) -a "data structure: arraylist"
	./$(TARGET) -f $(DATABASE) -a "data structure: singly linked list"
	./$(TARGET) -f $(DATABASE) -a "data structure: implement a doubly linked list"
	./$(TARGET) -f $(DATABASE) -a "data structure: hashmap"
	./$(TARGET) -f $(DATABASE) -a "data structure: 'random set'"
	./$(TARGET) -f $(DATABASE) -a "data structure: trie"
	./$(TARGET) -f $(DATABASE) -a "data structure: LRU"
	./$(TARGET) -f $(DATABASE) -a "data structure: avl tree"
	./$(TARGET) -f $(DATABASE) -a "data structure: red-black tree"
	./$(TARGET) -f $(DATABASE) -a "data structure: b-tree"
	./$(TARGET) -f $(DATABASE) -a "data structure: bloom filter"
	./$(TARGET) -f $(DATABASE) -a "algorithm: quicksort"
	./$(TARGET) -f $(DATABASE) -a "algorithm: mergesort"
	./$(TARGET) -f $(DATABASE) -a "algorithm: insertionsort"
	./$(TARGET) -f $(DATABASE) -a "algorithm: selectionsort"
	./$(TARGET) -f $(DATABASE) -a "algorithm: bubblesort"
	./$(TARGET) -f $(DATABASE) -a "algorithm: linear search over an array"
	./$(TARGET) -f $(DATABASE) -a "algorithm: binary search over an array, recursively"
	./$(TARGET) -f $(DATABASE) -a "algorithm: binary search over a tree, iteratively"
	./$(TARGET) -f $(DATABASE) -a "algorithm: dfs over a tree, iteratively"
	./$(TARGET) -f $(DATABASE) -a "algorithm: dfs over a graph, iteratively"
	./$(TARGET) -f $(DATABASE) -a "algorithm: dfs over a tree, recursively"
	./$(TARGET) -f $(DATABASE) -a "algorithm: dfs over a graph, recursively"
	./$(TARGET) -f $(DATABASE) -a "algorithm: dfs on adjacency list"
	./$(TARGET) -f $(DATABASE) -a "algorithm: bfs on adjacency matrix"
	./$(TARGET) -f $(DATABASE) -a "algorithm: bfs over a tree"
	./$(TARGET) -f $(DATABASE) -a "algorithm: bfs over a graph"
	./$(TARGET) -f $(DATABASE) -a "algorithm: dijkstra's"
	./$(TARGET) -f $(DATABASE) -a "algorithm: prim's"
	./$(TARGET) -f $(DATABASE) -a "algorithm: kruskal's"
	./$(TARGET) -f $(DATABASE) -a "algorithm: ford-fulkerson's max-flow / min-cut"
	./$(TARGET) -f $(DATABASE) -a "solve: 'two crystal balls' problem"
	./$(TARGET) -f $(DATABASE) -a "solve: 'coin change' problem"
	./$(TARGET) -f $(DATABASE) -a "solve: 'max subarray' problem"
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

