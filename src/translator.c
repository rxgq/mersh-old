#include <stdio.h>
#include "translator.h"

void translate(Token *tokens) {
    
}

void write_class(Token token) {
    char filename[260];

    snprintf(filename, sizeof(filename), "%s.txt", token.name);

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    fprintf(fptr, "Class: %s\n", token.name);

    fclose(fptr);
}