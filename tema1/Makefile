# IOCLA 2016
# USO remembers

CC=gcc
FLAGS=-fPIC -g -Wall -std=c89
MAIN=main.c
EXEC=tema1
OBJS=entry.o main.o priority_queue.o rbtree.o
HEADERS=entry.h priority_queue.h rbtree.h utils/utils.h
TOT=./1-multi/checker/multi/tot

build: $(EXEC)

test: $(EXEC)
	cp $(EXEC) ./1-multi/checker/multi/
	cp *.c ./1-multi/checker/multi/tot
	cp *.h ./1-multi/checker/multi/tot
	cp ./utils/* ./1-multi/checker/multi/tot
	cd 1-multi/checker/multi &&\
	./run_all.sh &&\
	cd -


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
