#include "tokenizer.h"

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

typedef struct {
    Token originator;
    Token recipient;
    Token relation;
} ClassRelation;

typedef struct {
    Token class_identifier;
} ClassDefinition;

void transform(Token *tokens);

#endif