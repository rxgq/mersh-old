#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "tokenizer.h"
#include "transformer.h"
#include "translator.h"

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case IDENTIFIER:  return "IDENTIFIER";
        case DECLARATION: return "DECLARATION";
        case OPEN_BRACE:  return "OPEN_BRACE";
        case CLOSE_BRACE: return "CLOSE_BRACE";
        case INHERITANCE: return "INHERITANCE";
        case COMPOSITION: return "COMPOSITION";
        case AGGREGATION: return "AGGREGATION";
        case ASSOCIATION: return "ASSOCIATION";
        case LINK:        return "LINK";
        case DEPENDENCY:  return "DEPENDENCY";
        case REALIZATION: return "REALIZATION";
        case UNKNOWN:     return "UNKNOWN";
        default:          return "INVALID_TOKEN_TYPE";
    }
}

void tokens_to_string(Token *tokens) {
    for (int i = 0; tokens[i].value != NULL; i++) {
        printf("%d: (line %d) Type %s | '%s' %s\n", i, tokens[i].line, token_type_to_string(tokens[i].type), tokens[i].value, tokens[i].name);
    }
}

void class_exprs_to_string(ClassExpressions *exprs) {
    printf("Class Definitions:\n");
    for (int i = 0; i < exprs->definition_count; i++) {
        printf("  Definition %d: %s\n", i + 1, exprs->definitions[i].identifier.value);
    }

    printf("\nClass Relations:\n");
    for (int i = 0; i < exprs->relation_count; i++) {
        printf("  Relation %d: %s %s %s\n", i + 1, exprs->relations[i].originator.value, exprs->relations[i].relation.value, exprs->relations[i].recipient.value);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }  

    FILE *fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc((sz + 1) * sizeof(char));
    if (buff == NULL) {
        perror("Memory allocation failed");
        fclose(fptr);
        return 1;
    }

    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';
    fclose(fptr);

    Token* tokens = tokenize(buff);
    if (tokens == NULL) {
        fprintf(stderr, "Tokenization failed\n");
        free(buff);
        return 1;
    }
    
    // tokens_to_string(tokens);

    ClassExpressions exprs;
    transform(tokens, &exprs);

    // class_exprs_to_string(&exprs);

    #ifdef _WIN32
    _mkdir("out");
    #else
    mkdir("out", 0777);
    #endif

    translate(&exprs);

    free(buff);
    free(tokens);

    return 0;
}