#include "mpc.h"
#define MAX 2048
struct tableEntry
{
    char identifier[MAX];
    char type[MAX];
};
int index;

int getType(char identifier[], struct tableEntry tableEntries[])
{
    for (int i = 0; i < MAX; i++)
    {
        if (strcmp(identifier, tableEntries[i].identifier) == 0)
        {
            return i;
        }
    }
    return -1;
}
bool vibe_check(char type[], char expected[])
{
    char localType[MAX];
    char localExpected[MAX];
    strcpy(localType, "");
    strcpy(localExpected, "");
    if (strstr(type, "[]") != NULL)
    {
        for (int i = 0; i < MAX; i++)
        {
            if (!(type[i] == '[' || type[i] == ']') && type[i] != '\0')
            {
                append(localType, type[i]);
            }
        }
    }
    else
    {
        strcpy(localType, type);
    }
    if (strstr(expected, "[]") != NULL)
    {
        for (int i = 0; i < MAX; i++)
        {
            if (!(expected[i] == '[' || expected[i] == ']') && expected[i] != '\0')
            {
                append(localExpected, expected[i]);
            }
        }
    }
    else
    {
        strcpy(localExpected, expected);
    }
    char group1[2][MAX] = {
        "char",
        "string"};
    char group2[2][MAX] = {
        "double",
        "float"};
    char group3[8][MAX] = {
        "byte",
        "sbyte",
        "int",
        "long",
        "short",
        "uint",
        "ulong",
        "ushort"};
    if (strcmp(localExpected, "bool") == 0)
    {
        if (strcmp(localExpected, localType) == 0)
            return true;
    }
    else if (strcmp(localExpected, "char") == 0 || strcmp(localExpected, "string") == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            if (strcmp(localType, group1[i]) == 0)
            {
                return true;
            }
        }
    }
    else if (strcmp(localExpected, "float") == 0 || strcmp(localExpected, "double") == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            if (strcmp(localType, group2[i]) == 0)
            {
                return true;
            }
        }
    }
    else if (strcmp(localExpected, "byte") == 0 || strcmp(localExpected, "sbyte") == 0 ||
             strcmp(localExpected, "int") == 0 || strcmp(localExpected, "uint") == 0 ||
             strcmp(localExpected, "long") == 0 || strcmp(localExpected, "ulong") == 0 ||
             strcmp(localExpected, "short") == 0 || strcmp(localExpected, "ushort") == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            if (strcmp(localType, group3[i]) == 0)
            {
                return true;
            }
        }
    }
    else if (strcmp(localExpected, "var") == 0)
    {
        return true;
    }
    return false;
}
bool exists(char identifier[], struct tableEntry tableEntries[])
{
    for (int i = 0; i < MAX; i++)
    {
        if (strcmp(identifier, tableEntries[i].identifier) == 0)
        {
            return true;
        }
    }
    return false;
}
bool findType(mpc_ast_t *parent, char who[], struct tableEntry tableEntries[])
{
    int childNumber = parent->children_num;
    char expType[MAX];
    if (strstr(parent->tag, "identifier") != NULL)
    {
        if (!exists(parent->contents, tableEntries))
        {
            printf("Identifier : '%s' was never declared!\n", parent->contents);
            exit(1);
        }
        if (parent->contents[0] != '\0')
        {
            if (!vibe_check(tableEntries[getType(parent->contents, tableEntries)].type, tableEntries[getType(who, tableEntries)].type))
            {
                printf("Conflicting types on '%s'\n", who);
                exit(1);
            }
        }
        return false;
    }
    else if (strstr(parent->tag, "binary_operator") != NULL)
    {
        if ((strcmp(tableEntries[getType(who, tableEntries)].type, "bool") == 0 ||
             strcmp(tableEntries[getType(who, tableEntries)].type, "string") == 0 ||
             strcmp(tableEntries[getType(who, tableEntries)].type, "char") == 0) &&
            !(strcmp(parent->contents, "<=") == 0 || strcmp(parent->contents, "==") == 0 || strcmp(parent->contents, ">=") == 0 ||
              strcmp(parent->contents, "<") == 0 || strcmp(parent->contents, ">") == 0))
        {
            printf("Invalid operation for type %s\n", tableEntries[getType(who, tableEntries)].type);
            exit(1);
        }
        else if ((strcmp(tableEntries[getType(who, tableEntries)].type, "double") == 0 || strcmp(tableEntries[getType(who, tableEntries)].type, "float") == 0) &&
                 !(strcmp(parent->contents, "+") == 0 || strcmp(parent->contents, "/") == 0 ||
                   strcmp(parent->contents, "*") == 0 || strcmp(parent->contents, "-") == 0))
        {
            printf("Invalid operation for type %s\n", tableEntries[getType(who, tableEntries)].type);
            exit(1);
        }
    }
    else if (strstr(parent->tag, "literal_expression") != NULL)
    {
        char localType[MAX];
        strcpy(localType, "");
        if (strstr(tableEntries[getType(who, tableEntries)].type, "[]") != NULL)
        {
            for (int i = 0; i < MAX; i++)
            {
                if (!(tableEntries[getType(who, tableEntries)].type[i] == '[' ||
                      tableEntries[getType(who, tableEntries)].type[i] == ']') &&
                    tableEntries[getType(who, tableEntries)].type[i] != '\0')
                {
                    append(localType, tableEntries[getType(who, tableEntries)].type[i]);
                }
            }
        }
        else
        {
            strcpy(localType, tableEntries[getType(who, tableEntries)].type);
        }
        if (strcmp(localType, "bool") == 0)
        {
            strcpy(expType, "bool_literal");
        }
        else if (strcmp(localType, "string") == 0)
        {
            strcpy(expType, "string_literal");
        }
        else if (strcmp(localType, "char") == 0)
        {
            strcpy(expType, "char_literal");
        }
        else if (strcmp(localType, "double") == 0 || strcmp(localType, "float") == 0)
        {
            strcpy(expType, "real_literal");
        }
        else if (strcmp(localType, "var") == 0)
        {
            strcpy(expType, "expression");
        }
        else
        {
            strcpy(expType, "integer_literal");
        }
        if (strstr(parent->tag, expType) == NULL)
        {
            printf("Conflicting types on '%s'\n", who);
            exit(1);
        }
        return false;
    }
    for (int i = 0; i < childNumber; i++)
    {
        if (!(strstr(parent->tag, "statement|assignment_statement") != NULL &&
              strstr(tableEntries[getType(who, tableEntries)].type, "[]") != NULL && i != 5))
        {
            findType(parent->children[i], who, tableEntries);
        }
    }
    return true;
}
void findBool(mpc_ast_t *parent, char type[], struct tableEntry tableEntries[])
{

    int childrenNum = parent->children_num;
    for (int i = 0; i < childrenNum; i++)
    {
        if (strstr(parent->children[i]->tag, "identifier") != NULL)
        {
            if (strcmp(type, tableEntries[getType(parent->children[i]->contents, tableEntries)].type) != 0)
            {
                printf("Conflicting types \n");
                exit(1);
            }
        }
        else if (strstr(parent->children[i]->tag, "binary_operator") != NULL)
        {
            if (!(strcmp("<=", parent->children[i]->contents) == 0 || strcmp(">=", parent->children[i]->contents) == 0 ||
                  strcmp("<", parent->children[i]->contents) == 0 || strcmp(">", parent->children[i]->contents) == 0))
            {
                printf("Invalid operator '%s'!\n", parent->children[i]->contents);
                exit(1);
            }
        }
        else if (strstr(parent->children[i]->tag, "literal_expression") != NULL)
        {
            char expType[MAX] = "";
            if (strcmp(type, "bool") == 0)
            {
                strcpy(expType, "bool_literal");
            }
            else if (strcmp(type, "string") == 0)
            {
                strcpy(expType, "string_literal");
            }
            else if (strcmp(type, "char") == 0)
            {
                strcpy(expType, "char_literal");
            }
            else if (strcmp(type, "double") == 0 || strcmp(type, "float") == 0)
            {
                strcpy(expType, "real_literal");
            }
            else if (strcmp(type, "var") == 0)
            {
                strcpy(expType, "expression");
            }
            else if (strstr(type, "|") != NULL)
            {
                if (strstr(type, "string_literal") != NULL)
                {
                    strcpy(expType, "string_literal");
                }
            }
            else
            {
                strcpy(expType, "integer_literal");
            }
            if (strstr(parent->children[i]->tag, expType) == NULL)
            {
                printf("Conflicting types %s\n", parent->children[i]->contents);
                exit(1);
            }
        }

        else if (strstr(parent->children[i]->tag, "binary_op_expression") != NULL)
        {
            findBool(parent->children[i], type, tableEntries);
        }
    }
}
void addToTable(mpc_ast_t *parent, char tag[], struct tableEntry tableEntries[])
{
    int child_number = parent->children_num;
    char currentVal[MAX];
    for (int i = 0; i < child_number; i++)
    {
        if (strstr(parent->tag, "declaration_statement") != NULL)
        {
            index++;
            appendString(tableEntries[index].type, parent->children[i]->contents);
        }
        else if (strstr(tag, "declaration_statement") != NULL && strcmp(parent->tag, "var_type|type|>") == 0)
        {
            appendString(tableEntries[index].type, parent->children[i]->contents);
        }
        else if (strstr(parent->tag, "assignment") != NULL)
        {
            char type[MAX];
            strcpy(type, tableEntries[getType(currentVal, tableEntries)].type);
            if (strstr(parent->children[i]->tag, "identifier") != NULL && strstr(parent->children[i]->tag, "expression") == NULL)
            {
                if (strstr(tag, "declaration_statement|>") != NULL)
                {
                    if (exists(parent->children[i]->contents, tableEntries))
                    {
                        printf("Identifier : '%s' was already declared!\n", parent->children[i]->contents);
                        exit(1);
                    }
                    appendString(tableEntries[index - 1].identifier, parent->children[i]->contents);
                }

                if (!exists(parent->children[i]->contents, tableEntries))
                {
                    printf("Identifier : '%s' was never declared!\n", parent->children[i]->contents);
                    exit(1);
                }
                strcpy(currentVal, parent->children[i]->contents);
            }
            if (strstr(parent->children[i]->tag, "expression"))
            {

                char expType[MAX];
                if (strstr(type, "[]") != NULL)
                {
                    strcpy(expType, "array_creation_expression");
                    if (strstr(parent->children[i]->tag, "array_creation_expression") != NULL)
                    {
                        if (strstr(type, parent->children[i]->children[1]->contents) == NULL)
                        {
                            printf("Wrong type for: %s\n", currentVal);
                            exit(1);
                        }
                    }
                }
                else if (strstr(parent->children[i]->tag, "cast_expression") != NULL)
                {
                    strcpy(expType, "cast_expression");
                    if (strcmp(type, parent->children[i]->children[1]->contents) != 0)
                    {
                        printf("Wrong type for: %s\n", currentVal);
                        exit(1);
                    }
                }
                else if (strcmp(type, "bool") == 0)
                {
                    strcpy(expType, "bool_literal");
                }
                else if (strcmp(type, "string") == 0)
                {
                    strcpy(expType, "string_literal");
                }
                else if (strcmp(type, "char") == 0)
                {
                    strcpy(expType, "char_literal");
                }
                else if (strcmp(type, "int") == 0 || strcmp(type, "byte") == 0 || strcmp(type, "sbyte") == 0 ||
                         strcmp(type, "long") == 0 || strcmp(type, "short") == 0 ||
                         strcmp(type, "ulong") == 0 || strcmp(type, "ushort") == 0 || strcmp(type, "uint") == 0)
                {
                    strcpy(expType, "integer_literal");
                }
                else if (strcmp(type, "double") == 0 || strcmp(type, "float") == 0)
                {
                    strcpy(expType, "real_literal");
                }
                else if (strcmp(type, "var") == 0)
                {
                    strcpy(expType, "expression");
                }
                if (strstr(parent->tag, expType) != NULL)
                {
                    if (strstr(parent->tag, "binary_op_expression") == NULL)
                    {
                        printf("Wrong type for: %s\n", currentVal);
                        exit(1);
                    }
                }
                else
                {
                    if (!(strcmp(expType, "cast_expression") == 0))
                    {
                        findType(parent, currentVal, tableEntries);
                    }
                }
            }
        }
        else if (strstr(parent->tag, "if") != NULL || strstr(parent->tag, "for") != NULL || strstr(parent->tag, "while") != NULL)
        {
            int childrenNum = parent->children_num;
            for (int i = 0; i < childrenNum; i++)
            {
                if (strstr(parent->children[i]->tag, "boolean_expression") != NULL)
                {
                    if (strstr(parent->children[i]->tag, "binary_op_expression") != NULL)
                    {
                        continue;
                    }
                    else if (strstr(parent->children[i]->tag, "unary_op_expression") != NULL)
                    {
                        if (strstr(parent->children[i]->children[0]->contents, "!") == NULL &&
                            strstr(parent->children[i]->children[0]->contents, "~") == NULL)
                        {
                            printf("Expression must be of boolean type!\n");
                            exit(1);
                        }
                        else if (exists(parent->children[i]->children[1]->contents, tableEntries))
                        {
                            if (strstr(tableEntries[getType(parent->children[i]->children[1]->contents, tableEntries)].type, "bool") == NULL)
                            {
                                printf("Expression must be of boolean type!\n");
                                exit(1);
                            }
                        }
                    }
                    else if (strstr(parent->children[i]->tag, "identifier") != NULL)
                    {
                        if (strstr(tableEntries[getType(parent->children[i]->contents, tableEntries)].type, "bool") == NULL)
                        {
                            printf("Expression must be of boolean type!\n");
                            exit(1);
                        }
                    }
                    else if (strstr(parent->children[i]->tag, "bool_literal") == NULL)
                    {
                        printf("Expression must be of boolean type!\n");
                        exit(1);
                    }
                }
            }
        }
        else if (strstr(parent->tag, "boolean_expression|expression|binary_op_expression") != NULL)
        {
            if (parent->children[0]->contents[0] != '\0')
            {
                findBool(parent, tableEntries[getType(parent->children[0]->contents, tableEntries)].type, tableEntries);
            }
            else
            {
                findBool(parent, parent->children[0]->tag, tableEntries);
            }
        }
        else if (strstr(parent->children[i]->tag, "unary_op_expression") != NULL)
        {
            char type[MAX];
            strcpy(type, tableEntries[getType(parent->children[i]->children[1]->contents, tableEntries)].type);
            if ((strcmp(type, "char") == 0 || strcmp(type, "string") == 0))
            {
                printf("Invalid operation for: %s\n", parent->children[i]->children[1]->contents);
                exit(1);
            }
            else if (strcmp(type, "bool") == 0 &&
                     !(strcmp(parent->children[i]->children[0]->contents, "!") == 0 || (strcmp(parent->children[i]->children[0]->contents, "~") == 0)))
            {
                printf("Invalid operation for: %s\n", parent->children[i]->children[1]->contents);
                exit(1);
            }
            else if ((strcmp(type, "int") == 0 || strcmp(type, "byte") == 0 || strcmp(type, "sbyte") == 0 || strcmp(type, "long") == 0 || strcmp(type, "short") == 0 ||
                      strcmp(type, "ulong") == 0 || strcmp(type, "ushort") == 0 || strcmp(type, "uint") == 0) &&
                     !(strcmp(parent->children[i]->children[0]->contents, "+") == 0 || (strcmp(parent->children[i]->children[0]->contents, "-") == 0) ||
                       (strcmp(parent->children[i]->children[0]->contents, "++") == 0) || (strcmp(parent->children[i]->children[0]->contents, "--") == 0)))
            {
                printf("Invalid operation for: %s\n", parent->children[i]->children[1]->contents);
                exit(1);
            }
            else if ((strcmp(type, "double") == 0 || strcmp(type, "float") == 0) &&
                     !(strcmp(parent->children[i]->children[0]->contents, "+") == 0 || (strcmp(parent->children[i]->children[0]->contents, "-") == 0) ||
                       (strcmp(parent->children[i]->children[0]->contents, "*") == 0) || (strcmp(parent->children[i]->children[0]->contents, "/") == 0)))
            {
                printf("Invalid operation for: %s\n", parent->children[i]->children[1]->contents);
                exit(1);
            }
        }
        else if (strstr(parent->children[i]->tag, "post_op_expression") != NULL)
        {
            char type[MAX];
            strcpy(type, tableEntries[getType(parent->children[i]->children[0]->contents, tableEntries)].type);
            if (strcmp(type, "bool") == 0 || strcmp(type, "char") == 0 || strcmp(type, "string") == 0 ||
                ((strstr(type, "[]") != NULL) && (strstr(parent->children[i]->children[0]->contents, "[") == NULL)))
            {
                printf("Invalid operation for: %s\n", parent->children[i]->children[0]->contents);
                exit(1);
            }
        }

        addToTable(parent->children[i], parent->tag, tableEntries);
    }
}

void printListTables(struct tableEntry tableEntries[])
{
    printf("Identifier list\n");
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
