CC=cl.exe
FLAGS=/MD /W3 /nologo /I.\utils
MAIN=main.c
EXEC=tema1.exe
OBJS=entry.obj main.obj priority_queue.obj rbtree.obj
HEADERS=entry.h priority_queue.h rbtree.h utils\utils.h utils\compare.h
TOT=.\1-multi\checker\multi\tot

build: $(EXEC)

test: $(EXEC)
	copy $(EXEC) 1-multi\checker\multi
	copy *.c 1-multi\checker\multi\tot
	copy *.h 1-multi\checker\multi\tot
	copy utils\* 1-multi\checker\multi\tot
	copy compare.dll 1-multi\checker\multi
	cd 1-multi\checker\multi && run.exe


entry.obj: entry.c $(HEADERS)
	$(CC) $(FLAGS) /c entry.c

rbtree.obj: rbtree.c $(HEADERS)
	$(CC) $(FLAGS) /c rbtree.c

priority_queue.obj: priority_queue.c $(HEADERS)
	$(CC) $(FLAGS) /c priority_queue.c

main.obj: $(MAIN) $(HEADERS)
	$(CC) $(FLAGS) /c $(MAIN)

$(EXEC): $(OBJS) $(HEADERS)
	$(CC) $(FLAGS) $(OBJS) compare.lib /Fe$@

run: build
	$(EXEC)

clean:
	del $(EXEC) *.obj
