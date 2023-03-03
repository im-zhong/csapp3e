# -S Displays the information contained in the file's section headers
# -s symtab
all: m.o swap.o
	readelf -S swap.o
	readelf -s swap.o

m.o: m.c
swap.o: swap.c

.PHONY: clean
clean:
	rm *.o
