# Kata Queue

A program to generate and operate on a persisted queue of tasks to do. I'm focusing on coding kata's right now, but this could be adapted for general todo's or other desired items.

## How it works

There is a persisted database containing a list of items and a queue of items. Both start empty. 

1. Add desired katas to the list
2. Generate a queue of katas to perform
3. Dequeue a kata from the queue and perform it
4. Repeat step 3 until the queue is empty
5. Repeat steps 2-4 as many times as you'd like

## Build

```bash
make init

# OR: if you want a clean database
make build
```

### Dependencies + Installations

TODO

## CLI

```bash
# create a new database
./bin/kq -f ./kata.db -n

# add new kata
./bin/kq -f ./kata.db -a "implement a min-heap"

# remove a kata (note: kata text has to match exactly)
./bin/kq -f ./kata.db -r "implement a min-heap"

# list all katas
./bin/kq -f ./kata.db -k 

# generate a new, random queue
./bin/kq -f ./kata.db -g 

# print the queue
./bin/kq -f ./kata.db -q 

# dequeue a kata to see what to perform next
./bin/kq -f ./kata.db -d

# clear the queue 
./bin/kq -f ./kata.db -c

# wipe the database 
./bin/kq -f ./kata.db -w
```
