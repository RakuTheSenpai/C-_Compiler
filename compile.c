#include "lexical.h"
#include "syntax.h"
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>

#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))

FILE *fp;

#define MAX 2048
struct Token tokens[MAX];
struct Node *syntaxTree;
struct tableEntry tableEntries[MAX];
int main(int argc, char **argv)
{
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("error while opening the file\n");
        exit(0);
    }

    tokenize(fp, tokens);
    // printTable(tokens);
    mpc_result_t r = makeAST(tokens);
    semantic(r.output, tableEntries);
}
