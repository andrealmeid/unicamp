CC=g++
CFLAGS=-I. -lstdc++
DEPS = pmr.h
OBJ = pmr.o pmr_e_h.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pmr: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) *~ pmr
