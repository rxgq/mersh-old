#include <stdio.h>
#include "translator.h"
#include <string.h>

int file_exists(FILE *fptr) {
    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    
    return sz == 0 ? 0 : 1;
}

const char* modifier_to_string(char modifier) {
    switch (modifier) {
        case '+': return "public";
        case '-': return "private";
        case '#': return "protected";
        case '~': return "internal";
        default:  return "";
    }
}

int is_method(const char *str) {
    size_t len = strlen(str);
    if (len < 2) {
        return 0;
    }

    return ((str[len - 2] == '(' && str[len - 1] == ')') || (str[len - 3] == '(' && str[len - 2] == ')'));
}

int is_static(const char *str) {
    return (str[strlen(str) - 1] == '$');
}

int is_abstract(const char *str) {
    return (str[strlen(str) - 1] == '*');
}

void write_class(ClassDefinition class_def) {
    char filename[256];
    snprintf(filename, sizeof(filename), "out/%s.cs", class_def.identifier.value);

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    fprintf(fptr, "public class %s {\n", class_def.identifier.value);
    for (int i = 0; i < class_def.property_count; ++i) {
        Property prop = class_def.properties[i];
        const char *modifier_str = modifier_to_string(prop.modifier);

        if (is_static(prop.identifier)) {
            int len = strlen(prop.identifier);
            prop.identifier[len-1] = '\0';
            
            if (is_method(prop.identifier)) {
                fprintf(fptr, "    %s static %s {\n\n\t}\n", modifier_str, prop.identifier + 1);
            } else {
                fprintf(fptr, "    %s static %s { get; set; }\n", modifier_str, prop.identifier + 1);
            }
        } 
        else if (is_abstract(prop.identifier)) {
            int len = strlen(prop.identifier);
            prop.identifier[len-1] = '\0';

            if (is_method(prop.identifier)) {
                fprintf(fptr, "    %s abstract %s {\n\n\t}\n", modifier_str, prop.identifier + 1);
            } else {
                fprintf(fptr, "    %s abstract %s { get; set; }\n", modifier_str, prop.identifier + 1);
            }
        }
        else {
            if (is_method(prop.identifier)) {
                fprintf(fptr, "    %s %s {\n\n\t}\n", modifier_str, prop.identifier + 1);
            } else {
                fprintf(fptr, "    %s %s { get; set; }\n", modifier_str, prop.identifier + 1);
            }
        }

    }
    fprintf(fptr, "}\n");

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
        write_class(exprs->definitions[i]);
    }

    for (int i = 0; i < exprs->relation_count; i++) {
        write_relation(&exprs->relations[i]);
    }
}