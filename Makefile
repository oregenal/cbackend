CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
BIN = server
SRC = src
OBJ = obj

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all:$(BIN)

release:CFLAGS=-std=c11 -O2 -DNDEBUG
release:clean
release:$(BIN)

$(OBJ):
	mkdir -p $@

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o:$(SRC)/%.c $(SRC)/%.h | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/main.o:$(SRC)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)/* $(BIN)
