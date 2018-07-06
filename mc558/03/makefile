CC := g++

CFLAGS := -std=c++11 -O3 -pedantic -Wall

COMPILE_COMMAND := $(CC) $(CFLAGS)

OUTPUT := lab03

all:
	$(COMPILE_COMMAND) -o $(OUTPUT) lab03.cpp solve.cpp

run: 
	./lab03 < arq1.in > out_inst1.txt
	./lab03 < arq2.in > out_inst2.txt
	./lab03 < arq3.in > out_inst3.txt


clean:
		rm -f *.o 
