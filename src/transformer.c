#include <stdio.h>
#include <string.h>
#include "transformer.h"
#include "tokenizer.h"


int is_class_definition(Token token) {
    return strcmp(token.value, "class") == 0;
}

// int is_relation(Token token) {
    
// }

Token* next_token(Token *tokens, int curr) {
    return &tokens[curr + 1];
}

void transform(Token *tokens) {

    int curr = 0;
    int current_line = tokens[curr].line;

    while (tokens[curr].name != NULL) {
        printf("Line %d\n", current_line);

        while (tokens[curr].name != NULL && tokens[curr].line == current_line) {
            printf("   %d: Value: '%s' | Name: %s\n", curr, tokens[curr].value, tokens[curr].name);
            curr++;
        }

        if (tokens[curr].name != NULL) {
            current_line = tokens[curr].line;
            printf("\n");
        }
    }

    printf("\n\n");

}
