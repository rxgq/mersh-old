#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// typedef struct {
//     char *name;
//     enum Accessor access_modifier;
//     enum Classifier item_modifier;
// } Property;

// typedef struct {
//     char *name;
//     enum Accessor access_modifier;
//     enum Classifier item_modifier;
//     char *return_type;
// } Method;

// enum Accessor {
//     PUBLIC,
//     PROTECTED,
//     PRIVATE,
//     PACKAGE
// };

enum TokenType {
    CLASS,
    IDENTIFIER,
    OPEN_BRACE,
    CLOSE_BRACE
};

typedef struct {
    char *name;
    enum TokenType type;
} Token;

char* trim(char* str) {
    while (isspace((unsigned char)*str)) 
        str++;

    if (*str == 0) 
        return str;

    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) 
        end--;

    end[1] = '\0';
    return str;
}

void Tokenize(char *source) {
    char* pch = strtok(source, "\r\n");

    int count = 0;
    while (pch != NULL)
    {
        char* trimmed = trim(pch);

        if (count == 0 && !strcmp(trimmed, "classDiagram") == 0) {
            // error, the first line should be "classDiagram"
        }

        printf("%d %s\n", count, trimmed);

        pch = strtok(NULL, "\r\n");
        count++;
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

    Tokenize(buff);

    free(buff);

    return 0;
}