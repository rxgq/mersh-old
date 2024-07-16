#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

int is_property(Token token) {
    return token.type == PROPERTY;
}

ClassExpressions* transform(Token *tokens, ClassExpressions *exprs) {
    int curr = 0;
    int current_line = tokens[curr].line;
    char *current_class = (char *)malloc(64 * sizeof(char));

    exprs->relation_count = 0;
    exprs->definition_count = 0;

    while (tokens[curr].name != NULL) {
        // printf("Line %d\n", current_line);

        while (tokens[curr].name != NULL && tokens[curr].line == current_line) {
            printf("   %d: Value: '%s' | Name: %s\n", curr, tokens[curr].value, tokens[curr].name);

            if (is_class_definition(tokens[curr])) {
                printf("      This token is a class definition.\n\n");

                ClassDefinition expr;
                expr.identifier = tokens[curr + 1];
                expr.property_count = 0;
                strcpy(current_class, tokens[curr + 1].value);

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
            else if (is_property(tokens[curr])) {
                printf("      This token is a property definition.\n\n");
                
                Property prop;
                prop.assigned_class = current_class;
                prop.identifier = tokens[curr].value;
                prop.modifier = tokens[curr].name[0];

                if (exprs->definition_count > 0) {
                    // exprs->definitions[exprs->definition_count - 1].properties[exprs->definitions[exprs->definition_count - 1].property_count++] = prop;
                }
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