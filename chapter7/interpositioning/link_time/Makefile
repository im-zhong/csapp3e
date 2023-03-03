all: mymalloc.o main.o
	gcc -Wl,--wrap,malloc -Wl,--wrap,free -o main.out $^

mymalloc.o: mymalloc.c
	gcc -DUSE_MY_MALLOC -c -o mymalloc.o mymalloc.c
main.o:

.PHONY: clean
clean:
	rm *.o main.out