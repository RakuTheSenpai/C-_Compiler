#include "mpc.h"
#define MAX 2048
struct tableEntry
{
    char identifier[2048];
    char type[2048];
};
int index;
// void getValue(mpc_ast_t *parent, char tag[], struct tableEntry tableEntries[])
// {
//     int child_number = parent->children_num;
//     appendString(tableEntries[index - 1].value, parent->contents);
//     for (int i = 0; i < child_number; i++)
//     {
//         getValue(parent->children[i], parent->tag, tableEntries);
//     }
// }
void exists(char identifier[], struct tableEntry tableEntries[])
{
    for (int i = 0; i < MAX; i++)
    {
        if (strcmp(identifier, tableEntries[i].identifier) == 0)
        {
            printf("Identifier \'%s\' Declared more than once\n", identifier);
            exit(1);
        }
    }
}
void addToTable(mpc_ast_t *parent, char tag[], struct tableEntry tableEntries[])
{
    int child_number = parent->children_num;
    for (int i = 0; i < child_number; i++)
    {
        if (strcmp(parent->tag, "statement|declaration_statement|>") == 0)
        {
            index++;
            appendString(tableEntries[index].type, parent->children[i]->contents);
        }
        else if (strcmp(tag, "statement|declaration_statement|>") == 0 && strcmp(parent->tag, "var_type|type|>") == 0)
        {
            appendString(tableEntries[index].type, parent->children[i]->contents);
        }
        else if (strcmp(tag, "statement|declaration_statement|>") == 0 && strcmp(parent->tag, "id_or_assignment|assignment_statement|>") == 0 && strcmp(parent->children[i]->tag, "identifier|regex") == 0)
        {
            exists(parent->children[i]->contents, tableEntries);
            appendString(tableEntries[index - 1].identifier, parent->children[i]->contents);
        }
        // else if (strcmp(tag, "statement|declaration_statement|>") == 0 && strcmp(parent->tag, "id_or_assignment|assignment_statement|>") == 0)
        // {
        //     if (strstr(parent->children[i]->tag, "expression") != NULL)
        //     {
        //         getValue(parent->children[i], tag, tableEntries);
        //     }
        // }
        addToTable(parent->children[i], parent->tag, tableEntries);
    }
}

void printListTables(struct tableEntry tableEntries[])
{
    for (int i = 0; i < MAX; i++)
    {
        if (tableEntries[i].type[0] != '\0')
        {
            printf("Type: %s Identifier: %s\n", tableEntries[i].type, tableEntries[i].identifier);
        }
    }
}
void semantic(mpc_ast_t *syntaxTree, struct tableEntry tableEntries[])
{
    index = -1;
    addToTable(syntaxTree, "", tableEntries);
    mpc_ast_delete(syntaxTree);
    printListTables(tableEntries);
}
