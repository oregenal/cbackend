CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
BIN = server
SRC = src
OBJ = obj

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

.PHONY:install clean

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

install:
	cp backend.service /etc/systemd/system/backend.service
	@echo "User=`stat -c '%U' .`" >> /etc/systemd/system/backend.service
	@echo "WorkingDirectory=`pwd`" >> /etc/systemd/system/backend.service
	@echo "ExecStart=`pwd`" >> /etc/systemd/system/backend.service

clean:
	rm -f $(OBJ)/* $(BIN)
