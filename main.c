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

    INHERITANCE,
    COMPOSITION,
    AGGREGATION,
    ASSOCIATION,
    LINK, // takes care of solid and dashed
    DEPENDENCY,
    REALIZATION,

    OPEN_BRACE,
    CLOSE_BRACE
};

typedef struct {
    char *name;
    char *value;
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

void tokenize_line(Token *tokens, char *line, int *count) {
    char* tok = trim(line);
    int len = strlen(tok);

    if (strcmp(tok, "classDiagram") == 0) {
        (*count)--;
        return;
    }

    int curr = 0;
    while (curr < len) {           // takes care of the aggregation o-- (picked up by isalnum)
        if (isalnum(tok[curr]) && (tok[curr != 0] && tok[curr + 1] != '-')) {
            int start = curr;
            
            while (isalnum(tok[curr])) {
                printf("%c", tok[curr]);
                curr++;
            }

            printf(" ");
        }

        else { 
            switch (tok[curr]) {
                case '<':
                    if (curr + 3 < len && 
                        tok[curr + 1] == '|' && 
                        tok[curr + 2] == '-' && 
                        tok[curr + 3] == '-'
                    ) {
                        (* tokens).name = "Inheritance";
                        (* tokens).value = "<|--";
                        (* tokens).type = INHERITANCE;
                        (* count)++;
                        curr += 4;
                    }

                    break;

                case '*':
                    if (tok[curr + 1] == '-' && tok[curr + 2] == '-') {
                        (* tokens).name = "Composition";
                        (* tokens).type = COMPOSITION;
                        (* tokens).value = "*--";
                        (* count)++;
                        curr += 3;
                    }
                    break;

                case 'o':
                    if (tok[curr + 1] == '-' && tok[curr + 2] == '-') {
                        (* tokens).name = "Aggregation";
                        (* tokens).type = AGGREGATION;
                        (* tokens).value = "o--";
                        (* count)++;
                        curr += 3;
                    }
                    break;

                case '-':
                    if (tok[curr + 1] == '-' && tok[curr + 2] == '>') {
                        (* tokens).name = "Association";
                        (* tokens).type = ASSOCIATION;
                        (* tokens).value = "-->";
                        (* count)++;
                        curr += 3;
                    }
                    else if (tok[curr + 1] == '-') {
                        (* tokens).name = "Link";
                        (* tokens).type = LINK;
                        (* tokens).value = "--";
                        (* count)++;
                        curr += 2;
                    }
                    break;
                case '.':
                    if (tok[curr + 1] == '.' && tok[curr + 2] == '>') {
                        (* tokens).name = "Dependency";
                        (* tokens).type = DEPENDENCY;
                        (* tokens).value = "..>";
                        (* count)++;
                        curr += 3;
                    }
                    else if (tok[curr + 1] == '.' && 
                        tok[curr + 2] == '|' &&
                        tok[curr + 3] == '>'
                    ) {
                        (* tokens).name = "Realization";
                        (* tokens).type = REALIZATION;
                        (* tokens).value = "..|>";
                        (* count)++;
                        curr += 3;
                    }
                    else if (tok[curr + 1] == '.') {
                        (* tokens).name = "Link";
                        (* tokens).type = LINK;
                        (* tokens).value = "..";
                        (* count)++;
                        curr += 3;
                    }
                    break;
            }
        }

        curr++;
    }

    printf("\n");

    // printf("\n%d %s\n", *count, tok);
}

Token* tokenize(char *source) {
    Token *tokens = (Token *)malloc(128 * sizeof(Token));
    if (tokens == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    

    int count = 0;
    char* line = strtok(source, "\r\n");
    while (line != NULL) {
        tokenize_line(tokens + count, line, &count);
        line = strtok(NULL, "\r\n");
    }

    tokens[count].value = NULL;

    return tokens;
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

    Token* tokens = tokenize(buff);
    for (int i = 0; tokens[i].value != NULL; i++) {
        printf("%d: Type %d | '%s' %s\n", i, tokens[i].type, tokens[i].value, tokens[i].name);
    }

    free(buff);

    return 0;
}