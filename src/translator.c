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

    fprintf(fptr, "public class %s {}\n", token.name);

    fclose(fptr);
}

void translate(ClassExpressions *exprs) {
    for (int i = 0; i < exprs->definition_count; i++) {
        write_class(exprs->definitions[i].identifier);
    }


}