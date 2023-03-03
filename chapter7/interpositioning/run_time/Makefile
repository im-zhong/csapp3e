all: mymalloc.so main.o
	gcc -o main.out main.o

mymalloc.so: mymalloc.c
	gcc -DUSE_MY_MALLOC -shared -fpic -o mymalloc.so mymalloc.c -ldl
main.o:

.PHONY: test
test: all
	LD_PRELOAD="./mymalloc.so" ./main.out

.PHONY: clean
clean:
	rm *.o *.so main.out
