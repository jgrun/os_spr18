CC = gcc
CFLAGS =-pthread -Wall -Wextra
DEPS = multi-lookup.h util.h lifo.h
OBJ = multi-lookup.o util.o lifo.o

%.o: %.c
	$(CC) -c $^ $(CFLAGS)

all: $(OBJ)
	$(CC) -o multilookup $^ $(CFLAGS)

clean:
	rm -f *.o *.gch multilookup results.txt serviced.txt
