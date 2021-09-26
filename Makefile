CC = gcc
CFLAGS = -Wall -ggdb
BIN = server
SRC = src
OBJ = obj

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/*.c, $(OBJ)/%.o, $(SRCS))

all:$(BIN)

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o:$(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@
