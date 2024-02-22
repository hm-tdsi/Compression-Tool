CC=gcc
CFLAGS=-I.

all: main test_functions clean_objects

main: main.o functions.o
	$(CC) -o main main.o functions.o $(CFLAGS)

test_functions: test_functions.o functions.o
	$(CC) -o test_functions test_functions.o functions.o $(CFLAGS)

main.o: main.c functions.h
	$(CC) -c main.c $(CFLAGS)

test_functions.o: test_functions.c functions.h
	$(CC) -c test_functions.c $(CFLAGS)

functions.o: functions.c functions.h
	$(CC) -c functions.c $(CFLAGS)

clean_objects:
	rm -f *.o

clean:
	rm -f main test_functions