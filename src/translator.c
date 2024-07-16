#include <stdio.h>
#include "translator.h"

int file_exists(FILE *fptr) {
    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    
    return sz == 0 ? 0 : 1;
}

void write_class(Token token) {
    char filename[256];
    snprintf(filename, sizeof(filename), "out/%s.cs", token.name);

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    if (file_exists(fptr) == 0) {
        fprintf(fptr, "public class %s {\n\n}\n", token.name);
    } else {
        //fseek(fptr, -2, SEEK_END);
        //fprintf(fptr, "\n}\n");
    }

    fclose(fptr);
}

// <|-- Inheritance: ClassA : ClassB
// *-- Composition: ClassA has-a ClassB: snytax: Bird *-- Feathers 
// o-- Aggregation: syntactically equivalent to Composition (The difference being aggregated classes can exist independently)
// others are irrelevant 
void write_relation(ClassRelation *relation) {
    char filename[256];

    snprintf(filename, sizeof(filename), "out/%s.cs", relation->originator.name);
    FILE *fptr = fopen(filename, "a");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    if (file_exists(fptr) == 0) {
        fprintf(fptr, "public class %s {\n\n}\n", relation->originator.name);
    }
    fclose(fptr);

    snprintf(filename, sizeof(filename), "out/%s.cs", relation->recipient.name);
    fptr = fopen(filename, "a");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    if (file_exists(fptr) == 0) {
        fprintf(fptr, "public class %s : %s {\n\n}\n", relation->recipient.name, relation->originator.name);
    } else {
        freopen(filename, "w", fptr);
        fprintf(fptr, "public class %s : %s {\n\n}\n", relation->recipient.name, relation->originator.name);
    }
    fclose(fptr);
}

void translate(ClassExpressions *exprs) {
    for (int i = 0; i < exprs->definition_count; i++) {
        write_class(exprs->definitions[i].identifier);
    }

    for (int i = 0; i < exprs->relation_count; i++) {
        write_relation(&exprs->relations[i]);
    }
}