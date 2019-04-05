
CC=gcc

build: libso_stdio.so

test: build
	cd _test; make clean; make

so_stdio.o: so_stdio.c
	$(CC) -fPIC -g -c so_stdio.c

libso_stdio.so: so_stdio.o
	gcc -shared so_stdio.o -o libso_stdio.so

clean:
	rm so_stdio.o libso_stdio.so
