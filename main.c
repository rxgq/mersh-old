#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    while (curr < len) {          // handles aggregation ungracefully
        if (isalnum(tok[curr]) && (tok[curr != 0] && tok[curr + 1] != '-')) {
            int start = curr;

            while (isalnum(tok[curr])) {
                curr++;
            }

            tokens[*count].value = (char *)malloc((curr - start + 1) * sizeof(char));
            strncpy(tokens[*count].value, tok + start, curr - start);
            tokens[*count].value[curr - start] = '\0';

            tokens[*count].name = (char *)malloc((curr - start + 1) * sizeof(char));
            strncpy(tokens[*count].name, tok + start, curr - start);
            tokens[*count].name[curr - start] = '\0';

            tokens[*count].type = IDENTIFIER;

            (*count)++;
        }
        else { 
            switch (tok[curr]) {
                case '<':
                    if (curr + 3 < len && 
                        tok[curr + 1] == '|' && 
                        tok[curr + 2] == '-' && 
                        tok[curr + 3] == '-'
                    ) {
                        tokens[*count].name = strdup("Inheritance");
                        tokens[*count].value = strdup("<|--");
                        tokens[*count].type = INHERITANCE;
                        (*count)++;
                        curr += 4;
                    }
                    break;

                case '*':
                    if (curr + 3 < len && 
                        tok[curr + 1] == '-' && 
                        tok[curr + 2] == '-'
                    ) {
                        tokens[*count].name = strdup("Composition");
                        tokens[*count].value = strdup("*--");
                        tokens[*count].type = COMPOSITION;
                        (*count)++;
                        curr += 3;
                    }
                    break;

                case 'o':
                    if (curr + 3 < len && 
                        tok[curr + 1] == '-' && 
                        tok[curr + 2] == '-'
                    ) {
                        tokens[*count].name = strdup("Aggregation");
                        tokens[*count].value = strdup("o--");
                        tokens[*count].type = AGGREGATION;
                        (*count)++;
                        curr += 3;
                    }
                    break;

                case '-':
                    if (curr + 3 < len && 
                        tok[curr + 1] == '-' && 
                        tok[curr + 2] == '>'
                    ) {
                        tokens[*count].name = strdup("Association");
                        tokens[*count].value = strdup("-->");
                        tokens[*count].type = ASSOCIATION;
                        (*count)++;
                        curr += 3;
                    }
                    else if (curr + 2 < len && tok[curr + 1] == '-') {
                        tokens[*count].name = strdup("Link");
                        tokens[*count].value = strdup("--");
                        tokens[*count].type = LINK;
                        (*count)++;
                        curr += 2;
                    }
                    break;

                case '.':
                    if (curr + 3 < len && 
                    tok[curr + 1] == '.' && 
                    tok[curr + 2] == '>'
                    ) {
                        tokens[*count].name = strdup("Dependency");
                        tokens[*count].value = strdup("..>");
                        tokens[*count].type = DEPENDENCY;
                        (*count)++;
                        curr += 3;
                    }
                    else if (curr + 4 < len && 
                        tok[curr + 1] == '.' && 
                        tok[curr + 2] == '|' && 
                        tok[curr + 3] == '>'
                    ) {
                        tokens[*count].name = strdup("Realization");
                        tokens[*count].value = strdup("..|>");
                        tokens[*count].type = REALIZATION;
                        (*count)++;
                        curr += 4;
                    }
                    else if (curr + 2 < len && tok[curr + 1] == '.') {
                        tokens[*count].name = strdup("Link");
                        tokens[*count].value = strdup("..");
                        tokens[*count].type = LINK;
                        (*count)++;
                        curr += 2;
                    }
                    break;
            }
        }

        curr++;
    }
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
        tokenize_line(tokens, line, &count);
        line = strtok(NULL, "\r\n");
    }

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
