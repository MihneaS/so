# IOCLA 2016
# USO remembers

CC=gcc
FLAGS=-fPIC -g -Wall -std=c89
MAIN=main.c
EXEC=tema1
OBJS=entry.o main.o priority_queue.o rbtree.o
HEADERS=entry.h priority_queue.h rbtree.h utils/utils.h

all: $(EXEC)
	cp $(EXEC) ./1-multi/checker/multi/

entry.o: entry.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

rbtree.o: rbtree.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

priority_queue.o: priority_queue.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

main.o: $(MAIN) $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

$(EXEC): $(OBJS) $(HEADERS)
	$(CC) $(FLAGS) $(OBJS) -o $@ -lcompare -L./utils

run: build
	./$(EXEC)

clean:
	rm $(EXEC) *.o
