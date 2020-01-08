CC=g++
CFLAGS=-O2
DEPS = hash.h nipdigests.h sha512.hh
OBJ = main.o nipdigests.o hash.o loadfile.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

whitevat_gcc: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o
