CC = gcc
CFLAGS = -Wall -Werror -g
BIN = utfconverter

SRC = $(wildcard *.c)

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

debug: $(SRC)
	$(CC) $(CFLAGS) -DCSE320 $^ -o $(BIN)


.PHONY: clean run

clean:
	rm -f *.o $(BIN)

run: $(BIN)
	./$(BIN)
