CC=gcc
CFLAGS=-Wall -g
INCLUDES=-I./src

SRCS=./src/main.c ./src/tokenizer.c ./src/transformer.c ./src/translator.c
OBJS=$(SRCS:.c=.o)
EXECUTABLES=main tokenizer transformer translator

all: $(EXECUTABLES) post-build

main: ./src/main.o ./src/tokenizer.o ./src/transformer.o ./src/translator.o
	$(CC) $(CFLAGS) $(INCLUDES) ./src/main.o ./src/tokenizer.o ./src/transformer.o ./src/translator.o -o main

tokenizer: ./src/tokenizer.o
	$(CC) $(CFLAGS) $(INCLUDES) ./src/tokenizer.o -o tokenizer

transformer: ./src/transformer.o
	$(CC) $(CFLAGS) $(INCLUDES) ./src/transformer.o -o transformer

translator: ./src/translator.o
	$(CC) $(CFLAGS) $(INCLUDES) ./src/translator.o -o translator


./src/%.o: ./src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLES)
