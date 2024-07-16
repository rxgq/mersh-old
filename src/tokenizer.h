#ifndef TOKENIZER_H
#define TOKENIZER_H

enum TokenType {
    IDENTIFIER,

    DECLARATION,
    OPEN_BRACE,
    CLOSE_BRACE,

    INHERITANCE,
    COMPOSITION,
    AGGREGATION,
    ASSOCIATION,
    LINK,
    DEPENDENCY,
    REALIZATION,

    UNKNOWN
};

typedef struct {
    char *name;
    char *value;
    int line;
    enum TokenType type;
} Token;

Token* tokenize(char *source);

#endif