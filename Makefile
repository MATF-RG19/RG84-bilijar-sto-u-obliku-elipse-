LDLIBS  = -lglut -lGLU -lGL -lm

main: main.o
	gcc -Wall main.o -o main $(LDLIBS)
main.o: main.c
	gcc -c main.c
clean:  
	rm *.o main


