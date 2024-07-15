#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    fseek(fptr, 0, SEEK_SET);

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

    for (int i = 0; tokens[i].value != NULL; i++) {
        printf("%d: Type %d | '%s' %s\n", i, tokens[i].type, tokens[i].value, tokens[i].name);

        free(tokens[i].value);
        free(tokens[i].name);
    }

    free(buff);
    free(tokens);

    return 0;
}
