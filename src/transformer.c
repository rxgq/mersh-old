#include <stdio.h>
#include <string.h>
#include "transformer.h"
#include "tokenizer.h"

int is_class_definition(Token token) {
    return strcmp(token.value, "class") == 0;
}

int is_relation_definition(Token token) {
    return strcmp(token.value, "<|--") == 0  || strcmp(token.value, "*--") == 0 || 
            strcmp(token.value, "o--") == 0  || strcmp(token.value, "-->") == 0 || 
            strcmp(token.value, "--") == 0   || strcmp(token.value, "..>") == 0 ||
            strcmp(token.value, "..|>") == 0 || strcmp(token.value, "..") == 0;
}

ClassExpressions* transform(Token *tokens, ClassExpressions *exprs) {
    int curr = 0;
    int current_line = tokens[curr].line;
    int inside_curly_braces = 0;

    exprs->relation_count = 0;
    exprs->definition_count = 0;

    while (tokens[curr].name != NULL) {
        printf("Line %d\n", current_line);

        while (tokens[curr].name != NULL && tokens[curr].line == current_line) {
            printf("   %d: Value: '%s' | Name: %s\n", curr, tokens[curr].value, tokens[curr].name);

            if (strcmp(tokens[curr].value, "{") == 0) {
                inside_curly_braces = 1;
            } 
            else if (strcmp(tokens[curr].value, "}") == 0) {
                inside_curly_braces = 0;
            }

            if (is_class_definition(tokens[curr])) {
                printf("      This token is inside curly braces or a class definition.\n\n");

                ClassDefinition expr;
                expr.identifier = tokens[curr + 1];

                exprs->definitions[exprs->definition_count++] = expr;
            } 
            else if (is_relation_definition(tokens[curr])) {
                printf("      This token is a relation definition.\n\n");
                
                ClassRelation expr;
                expr.originator = tokens[curr - 1];
                expr.relation = tokens[curr];
                expr.recipient = tokens[curr + 1];

                exprs->relations[exprs->relation_count++] = expr;
            } 
            else {
                printf("      This token is an identifier.\n\n");

            }

            curr++;
        }

        if (tokens[curr].name != NULL) {
            current_line = tokens[curr].line;
            printf("\n");
        }
    }

    return exprs;
}