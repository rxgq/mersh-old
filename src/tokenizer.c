#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void on_alnum(int *curr, char* tok, Token* tokens, int *count, int tok_line) {
    int start = *curr;

    while (isalnum(tok[*curr])) {
        (*curr)++;
    }

    int length = *curr - start;
    char* name = (char *)malloc((length + 1) * sizeof(char));
    strncpy(name, tok + start, length);
    name[length] = '\0';

    tokens[*count].type = strcmp(name, "class") == 0 ? DECLARATION : IDENTIFIER;

    tokens[*count].value = (char *)malloc((length + 1) * sizeof(char));
    strncpy(tokens[*count].value, tok + start, length);
    tokens[*count].value[length] = '\0';

    tokens[*count].name = (char *)malloc((length + 1) * sizeof(char));
    strncpy(tokens[*count].name, tok + start, length);
    tokens[*count].name[length] = '\0';

    tokens[*count].line = tok_line;

    (*count)++;
    free(name);
}

void on_brace(int *curr, char* tok, Token* tokens, int *count, int tok_line, char* source) {
    tokens[*count].value = (char *)malloc(2 * sizeof(char));
    tokens[*count].value[0] = tok[*curr];
    tokens[*count].value[1] = '\0';

    tokens[*count].name = (char *)malloc(2 * sizeof(char));
    tokens[*count].name[0] = tok[*curr];
    tokens[*count].name[1] = '\0';

    tokens[*count].type = tok[*curr] == '{' ? OPEN_BRACE : CLOSE_BRACE;
    tokens[*count].line = tok_line;

    (*curr)++;
    (*count)++;
}

void on_relation(int *curr, char* tok, Token* tokens, int *count, int tok_line) {
    tokens[*count].line = tok_line;

    switch (tok[*curr]) {
        case '<':
            if (tok[*curr + 1] == '|' && tok[*curr + 2] == '-' && tok[*curr + 3] == '-') {
                tokens[*count].name = strdup("Inheritance");
                tokens[*count].value = strdup("<|--");
                tokens[*count].type = INHERITANCE;
                (*count)++;
                (*curr) += 4;
            }
            break;

        case '*':
            if (tok[*curr + 1] == '-' && tok[*curr + 2] == '-') {
                tokens[*count].name = strdup("Composition");
                tokens[*count].value = strdup("*--");
                tokens[*count].type = COMPOSITION;
                (*count)++;
                (*curr) += 3;
            }
            break;

        case 'o':
            if (tok[*curr + 1] == '-' && tok[*curr + 2] == '-') {
                tokens[*count].name = strdup("Aggregation");
                tokens[*count].value = strdup("o--");
                tokens[*count].type = AGGREGATION;
                (*count)++;
                (*curr) += 3;
            }
            break;

        case '-':
            if (tok[*curr + 1] == '-' && tok[*curr + 2] == '>') {
                tokens[*count].name = strdup("Association");
                tokens[*count].value = strdup("-->");
                tokens[*count].type = ASSOCIATION;
                (*count)++;
                (*curr) += 3;
            }
            else if (tok[*curr + 1] == '-') {
                tokens[*count].name = strdup("Link");
                tokens[*count].value = strdup("--");
                tokens[*count].type = LINK;
                (*count)++;
                (*curr) += 2;
            }
            break;

        case '.':
            if (tok[*curr + 1] == '.' && tok[*curr + 2] == '>') {
                tokens[*count].name = strdup("Dependency");
                tokens[*count].value = strdup("..>");
                tokens[*count].type = DEPENDENCY;
                (*count)++;
                (*curr) += 3;
            }
            else if (tok[*curr + 1] == '.' && tok[*curr + 2] == '|' && tok[*curr + 3] == '>') {
                tokens[*count].name = strdup("Realization");
                tokens[*count].value = strdup("..|>");
                tokens[*count].type = REALIZATION;
                (*count)++;
                (*curr) += 4;
            }
            else if (tok[*curr + 1] == '.') {
                tokens[*count].name = strdup("Link");
                tokens[*count].value = strdup("..");
                tokens[*count].type = LINK;
                (*count)++;
                (*curr) += 2;
            }
            break;
    }
}

void on_attribute(int *curr, char* tok, Token* tokens, int *count, int tok_line) {
    int start = *curr;
    enum TokenType type = tok[*curr] == '+' ? PROPERTY : PROPERTY;

    while (tok[*curr] != '\0' && tok[*curr] != '\n' && tok[*curr] != '\r') {
        (*curr)++;
    }

    int length = *curr - start;
    char* attribute = (char *)malloc((length + 1) * sizeof(char));
    strncpy(attribute, tok + start, length);
    attribute[length] = '\0';

    tokens[*count].type = type;
    tokens[*count].value = (char *)malloc((length + 1) * sizeof(char));
    strncpy(tokens[*count].value, attribute, length);
    tokens[*count].value[length] = '\0';

    tokens[*count].name = (char *)malloc((length + 1) * sizeof(char));
    strncpy(tokens[*count].name, attribute, length);
    tokens[*count].name[length] = '\0';

    tokens[*count].line = tok_line;

    (*count)++;
    free(attribute);
}


void tokenize_line(Token *tokens, char *line, int *count, int tok_line, char* source) {
    char* tok = trim(line);
    int len = strlen(tok);

    if (strcmp(tok, "classDiagram") == 0) {
        (*count)--;
        return;
    }

    int curr = 0;
    if (tok[curr] == '+' || tok[curr] == '-') {
        on_attribute(&curr, tok, tokens, count, tok_line);
    }
    else {
        while (curr < len) {          
            if (isalnum(tok[curr]) && (tok[curr != 0] && tok[curr + 1] != '-')) {
                on_alnum(&curr, tok, tokens, count, tok_line);
            }
            else if (tok[curr] == '{' || tok[curr] == '}') {
                on_brace(&curr, tok, tokens, count, tok_line, source);
            }
            else { 
                on_relation(&curr, tok, tokens, count, tok_line);
            }

            curr++;
        }
    }
}

Token* tokenize(char *source) {
    Token *tokens = (Token *)malloc(256 * sizeof(Token));
    if (tokens == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    int count = 0;
    int tok_line = 1;
    char* line = strtok(source, "\r\n");
    while (line != NULL) {
        tokenize_line(tokens, line, &count, tok_line, source);
        line = strtok(NULL, "\r\n");

        tok_line++;
    }

    return tokens;
}