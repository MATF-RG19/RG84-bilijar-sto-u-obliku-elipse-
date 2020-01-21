PROGRAM   = zadatak
CC        = gcc
CFLAGS    = -g 
LDFLAGS   = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o
	$(CC) -o $(PROGRAM) main.o $(LDFLAGS)

.PHONY: clean dist

clean:
	-rm -f *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

run:
	./$(PROGRAM)

