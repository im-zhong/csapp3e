# Chapter3 - Machine-Level Representation of Programs

**TODO**
- write a Makefile for ProgramEncodings

**TIPS**
- use `gcc-S src.c` or `gcc-S src.c` to generate assembly files, you may use optimization level `-Og` or `-O`
- use `gcc -c src.c` to generate relocatable object files
- use `gcc -o executable *.o *.c` to link then generate executable file, be careful of dependency relationship, the dependent appears later
- use `objdump -d` or `gdb disassemble` to to disassemble the object or exetuable files
