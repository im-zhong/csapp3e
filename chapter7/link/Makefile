# ld -o prog main.o sum.o
# directly run this command can not link correctly, because you lost some important system object files and args
# but I do not know which I lost now, so I just let gcc do it for me
all: sum main
	cc -o prog main.o sum.o

# gcc atcually is a compiler driver, it normally dose preprocessing, complition, assembly and linking
# see -c -S -E options for gcc in manual
# 1. c preprocessor(cpp), which translates the C source file src.c into itermediate file src.i
# 2. c compiler(cc), which translates the src.i into assembly language src.s
# 3. assembler(as), which translates src.s into a binary relocatable object file src.o
# 4. linker program(ld), combines several object.o to create the executable object file <prog>
# 5. shell> prog. the shell invoke the os loader, which load data and code into memory and transfer control to the beginning of the program
sum: sum.c
	cpp sum.c sum.i
	cc -x cpp-output -S -o sum.s sum.i
	as -o sum.o sum.s

main: main.c
	cpp main.c main.i
	cc -x cpp-output -S -o main.s main.i
	as -o main.o main.s

.PHONY: clean
clean:
	rm *.i *.s *.o prog
