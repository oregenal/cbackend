CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
BIN = server
SRC = src
OBJ = obj
LIBS = -lcrypto

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

.PHONY:install clean all

all:$(BIN)

release:CFLAGS=-std=c11 -O2 -DNDEBUG
release:clean
release:$(BIN)

$(OBJ):
	mkdir -p $@

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(OBJ)/%.o:$(SRC)/%.c $(SRC)/%.h | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/main.o:$(SRC)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

install:
	cp backend.service /etc/systemd/system/backend.service
	@echo "User=`stat -c '%U' .`" >> /etc/systemd/system/backend.service
	@echo "WorkingDirectory=`pwd`" >> /etc/systemd/system/backend.service
	@echo "ExecStart=`pwd`/$(BIN)" >> /etc/systemd/system/backend.service

clean:
	rm -f $(OBJ)/* $(BIN)
