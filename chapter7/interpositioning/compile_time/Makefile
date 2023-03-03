all: mymalloc.o
	gcc -I. -o main.out main.c mymalloc.o

mymalloc.o: mymalloc.c
	gcc -DUSE_MY_MALLOC -c -o mymalloc.o mymalloc.c

.PHONY: clean
clean:
	rm *.o main.out