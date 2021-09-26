CC = gcc
CFLAGS = -Wall -ggdb
BIN = server
SRC = src
OBJ = obj

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all:$(BIN)

release:CFLAGS=-Wall -O2 -DNDEBUG
release:clean
release:$(BIN)

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o:$(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)/* $(BIN)
