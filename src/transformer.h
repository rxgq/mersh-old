#include "tokenizer.h"

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

typedef struct {
    Token originator;
    Token recipient;
    Token relation;
} ClassRelation;

typedef struct {
    Token identifier;
} ClassDefinition;

typedef struct {
    ClassRelation relations[64];
    int relation_count;
    ClassDefinition definitions[64];
    int definition_count;
} ClassExpressions;


ClassExpressions* transform(Token *tokens, ClassExpressions *exprs);

#endif