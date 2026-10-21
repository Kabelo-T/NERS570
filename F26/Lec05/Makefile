CC = gcc

OBJ = \
kabelo.o \
ffarha.o

all: main

%.o: %.c
	echo $(CC) -c $< -o $@ && cp $< $@

main: $(OBJ)
	echo $(CC) $(OBJ) -o main && touch main

clean:
	rm -rf *.o main
