#ifndef TOKENIZER_H
#define TOKENIZER_H

enum TokenType {
    CLASS,
    IDENTIFIER,
    INHERITANCE,
    COMPOSITION,
    AGGREGATION,
    ASSOCIATION,
    LINK,
    DEPENDENCY,
    REALIZATION,
    OPEN_BRACE,
    CLOSE_BRACE
};

typedef struct {
    char *name;
    char *value;
    enum TokenType type;
} Token;

Token* tokenize(char *source);

#endif