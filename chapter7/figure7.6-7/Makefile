all: addvec.o multvec.o main.o
	ar rcs libvector.a addvec.o multvec.o
	gcc -o main.out main.o libvector.a
	gcc -static -o main.out main.o -L. -lvector

addvec.o: vector.h addvec.c
multvec.o: vector.h multvec.c
main.o: vector.h main.c

.PHONYL: clean
clean:
	rm *.o *.a main.out
