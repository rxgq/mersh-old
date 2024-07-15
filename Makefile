CC=gcc
CFLAGS=-Wall -g
INCLUDES=-I./src

SRCS=./src/main.c ./src/tokenizer.c
OBJS=$(SRCS:.c=.o)
EXECUTABLES=main tokenizer

all: $(EXECUTABLES)

main: ./src/main.o ./src/tokenizer.o
	$(CC) $(CFLAGS) $(INCLUDES) ./src/main.o ./src/tokenizer.o -o main

tokenizer: ./src/tokenizer.o
	$(CC) $(CFLAGS) $(INCLUDES) ./src/tokenizer.o -o tokenizer

./src/%.o: ./src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLES)
