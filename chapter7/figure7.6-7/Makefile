all: addvec.o multvec.o main.o
	ar rcs libvector.a addvec.o multvec.o
	gcc -o main.out main.o libvector.a
	gcc -static -o main.out main.o -L. -lvector

shared: addvec.c multvec.c main.c
	gcc -shared -fpic -o libvector.so addvec.c multvec.c
	gcc -o main.out main.c ./libvector.so

addvec.o: vector.h addvec.c
multvec.o: vector.h multvec.c
main.o: vector.h main.c

.PHONYL: clean
clean:
	rm *.o *.a *.so main.out
