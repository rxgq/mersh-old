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

void transform(Token *tokens) {
    int curr = 0;
    int current_line = tokens[curr].line;
    int inside_curly_braces = 0;

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

            if (inside_curly_braces || is_class_definition(tokens[curr])) {
                printf("      This token is inside curly braces or a class definition.\n\n");


            } 
            else if (is_relation_definition(tokens[curr])) {
                printf("      This token is a relation definition.\n\n");


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

    printf("\n\n");
}
