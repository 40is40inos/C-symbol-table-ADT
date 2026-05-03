CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -Iinclude -O2
SRC = src/symbol_table.c
OBJ = $(SRC:.c=.o)
LIBRARY = libsymtable.a

all: $(LIBRARY)

$(LIBRARY): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(LIBRARY)

.PHONY: all clean
