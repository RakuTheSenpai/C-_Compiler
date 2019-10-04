#include "lexical.h"
#include <stdio.h>
#include <stdlib.h>

#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))

FILE *fp;

#define MAX 2048

struct Token tokens[MAX];
int main(int argc, char **argv) {
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("error while opening the file\n");
        exit(0);
    }
    tokenize(fp, tokens);
    for (int i = 0; i < LEN(tokens); i++) {
        if (strlen(tokens[i].lexem) != 0) {
            printf("%s - %s\n", tokens[i].lexem, tokens[i].type);
        }
    }
}