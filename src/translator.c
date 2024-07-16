#include <stdio.h>
#include "translator.h"

void write_class(Token token) {
    char filename[256];
    snprintf(filename, sizeof(filename), "out/%s.txt", token.name);

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    // check if file is newly created
    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    if (sz == 0) {
        fprintf(fptr, "public class %s {\n\n}\n", token.name);
    } else {
        fseek(fptr, -2, SEEK_END);
        fprintf(fptr, "\n}\n");
    }


    fclose(fptr);
}

void translate(ClassExpressions *exprs) {
    for (int i = 0; i < exprs->definition_count; i++) {
        write_class(exprs->definitions[i].identifier);
    }


}