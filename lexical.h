#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 2048
#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))
struct Token {
    int position;
    int line;
    char lexem[MAX];
    char type[MAX];
    char print[MAX];
};
//Keywords and other reserved symbols
char keywords[78][MAX] = {"abstract", "as", "base", "bool",
                          "break", "byte", "case", "catch",
                          "char", "checked", "class", "const",
                          "continue", "decimal", "default", "delegate",
                          "do", "double", "else", "enum",
                          "event", "explicit", "extern", "false",
                          "finally", "fixed", "float", "for",
                          "foreach", "goto", "if", "implicit",
                          "in", "int", "interface", "internal",
                          "is", "lock", "long", "namespace",
                          "new", "null", "object", "operator",
                          "out", "override", "params", "private",
                          "protected", "public", "readonly", "ref",
                          "return", "sbyte", "sealed", "short",
                          "sizeof", "stackalloc", "static", "string",
                          "struct", "switch", "this", "throw",
                          "true", "try", "typeof", "uint",
                          "ulong", "unchecked", "unsafe", "ushort",
                          "using", "using static", "virtual", "void",
                          "volatile", "while"};
char operators[33][MAX] = {
    "==", ">", ">=", "<",
    "<=", "=", "+", "-",
    "/", "*", "++", "--",
    "!", "~", "<<", ">>",
    "+=", "-=", "*=", "/=",
    "&&", "||", "!=", "%",
    "&", "|", "^", "%=",
    "<<=", ">>=", "&=", "^=",
    "|="};

char punctuators[10] = {';', '{', '}', '(', ')', '[', ']', '.', ',', ':'};
//Checks if Char belongs to the delimiter array
int isPunctuator(char str) {
    for (int i = 0; i < LEN(punctuators); i++) {
        if (str == punctuators[i]) {
            return 1;
        }
    }
    return 0;
}
//Appends Chars to string
void append(char *s, char c) {
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}
//Checks if Word belongs to the operator array
int isOperator(char word[]) {
    for (int i = 0; i < LEN(operators); i++) {
        if (strcmp(word, operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
//Checks if Character is of the operator type
int isOperatorDelim(char str) {
    char tempWord[MAX] = "";
    append(tempWord, str);
    for (int i = 0; i < LEN(operators); i++) {
        if (strcmp(tempWord, operators[i]) == 0) {
            tempWord[0] = '\0';
            return 1;
        }
    }
    tempWord[0] = '\0';
    return 0;
}
//Checks if Word belongs to the keyword array
int isKeyword(char word[]) {
    for (int i = 0; i < LEN(keywords); i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
//Checks if string is number
int isNumber(char word[]) {
    regex_t re;
    if (regcomp(&re, "^[0-9]+", REG_EXTENDED) != 0)
        return 0;
    int status = regexec(&re, word, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}
//Checks if string is double
int isDouble(char word[]) {
    regex_t re;
    if (regcomp(&re, "(([0-9]+)?\\.[0-9]*)", REG_EXTENDED) != 0)
        return 0;
    int status = regexec(&re, word, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}
//Checks if string is identifier
int isIdentifier(char word[]) {
    regex_t re;
    if (regcomp(&re, "^@?[_a-zA-Z][_a-zA-Z0-9]*", REG_EXTENDED) != 0)
        return 0;
    int status = regexec(&re, word, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}
//Checks if string is string literal
int isString(char word[]) {
    regex_t re;
    if (regcomp(&re, "\".*\"", REG_EXTENDED | REG_NOSUB) != 0)
        return 0;
    int status = regexec(&re, word, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}
//Checks if string is char literal
int isChar(char word[]) {
    regex_t re;
    if (regcomp(&re, "'(.|\\\\[0rnt])'", REG_EXTENDED | REG_NOSUB) != 0)
        return 0;
    int status = regexec(&re, word, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}
//Checks if string is single line comment
int isSingleLineComment(char word[]) {
    regex_t re;
    if (regcomp(&re, "//.*", REG_EXTENDED | REG_NOSUB) != 0)
        return 0;
    int status = regexec(&re, word, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}
//Checks if string is multi line comment
int isMultiLineComment(char word[]) {
    regex_t re;
    if (regcomp(&re, "\\/\\*.*\\*\\/", REG_EXTENDED | REG_NOSUB) != 0)
        return 0;
    int status = regexec(&re, word, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}
void tokenize(FILE *fp, struct Token tokens[MAX]) {
    printf("Tokenize Process\n");
    //Flags for specific cases
    int stringFlag = 0;
    int operatorFlag = 0;
    int numericFlag = 0;
    int singleFlag = 0;
    int multiFlag = 0;
    int charFlag = 0;

    int ch;                      //Character
    int count = 0;               //Char Position
    int wordStart = 0;           //Word Position
    int line = 1;                //Line Position
    char word[MAX] = "";         //Temporary Word Storage
    char tempOperator[MAX] = ""; //Temporary Operator Storage
    int index = 0;               //Index for Tokens array

    while ((ch = fgetc(fp)) != EOF) {
        /* Particular cases */
        if (word[0] == '\0') {
            wordStart = count;
        }
        if (!isOperatorDelim(ch) && operatorFlag) {
            if (isOperator(word)) {
                struct Token t;
                t.position = wordStart;
                t.line = line;
                strncpy(t.type, "OPP", sizeof(t.type));
                strncpy(t.print, "Operator or Punctuator", sizeof(t.print));
                strncpy(t.lexem, word, sizeof(t.lexem));
                tokens[index] = t;
                index++;
                // printf("%s -> Operator or Punctuator at Position %d and Line %d\n", word, wordStart, line);
                word[0] = '\0';
            }
            operatorFlag = 0;
        }
        if (isdigit(ch) || (numericFlag == 1 && ch == '.')) {
            numericFlag = 1;
        } else {
            numericFlag = 0;
        }
        if (isSingleLineComment(word)) {
            singleFlag = 1;
        }
        if (ch == '\n' && singleFlag == 1) {
            word[0] = '\0';
            singleFlag = 0;
            // printf("Comment ignored\n");
        }
        if (strcmp(word, "/*") == 0) {
            multiFlag = 1;
        }
        if (ch == '/' && multiFlag == 1) {
            append(word, ch);
            printf("Pointer: %c,Word: %s\n", ch, word);
            multiFlag = 0;
            // printf("Comment ignored\n");
        }
        if (ch == '\"') {
            stringFlag = stringFlag == 0 ? 1 : 0;
        }
        if (ch == '\'') {
            charFlag = charFlag == 0 ? 1 : 0;
        }
        /*Word Processing*/
        if ((ch == ' ' || isPunctuator(ch) || isOperatorDelim(ch)) &&
            (!numericFlag && !stringFlag && !singleFlag && !multiFlag && !operatorFlag && !charFlag)) {
            if (word[0] != '\0') {
                if (isKeyword(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.print, "Keyword", sizeof(t.print));
                    if (strcmp(word, "if") == 0) {
                        strncpy(t.type, "IF", sizeof(t.type));
                    } else if (strcmp(word, "for") == 0) {
                        strncpy(t.type, "FOR", sizeof(t.type));
                    } else if (strcmp(word, "foreach") == 0) {
                        strncpy(t.type, "FOREACH", sizeof(t.type));
                    } else if (strcmp(word, "while") == 0) {
                        strncpy(t.type, "WHILE", sizeof(t.type));
                    } else if (strcmp(word, "switch") == 0) {
                        strncpy(t.type, "SWITCH", sizeof(t.type));
                    } else {
                        strncpy(t.type, "K", sizeof(t.type));
                    }
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    // printf("%s -> Keyword at Position %d and Line %d\n", word, wordStart, line);
                    word[0] = '\0';
                } else if (isDouble(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "RL", sizeof(t.type));
                    strncpy(t.print, "Real-Literal", sizeof(t.print));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    // printf("%s -> Real-Literal at Position %d and Line %d\n", word, wordStart, line);
                    word[0] = '\0';
                } else if (isNumber(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "IL", sizeof(t.type));
                    strncpy(t.print, "Integer-Literal", sizeof(t.print));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    // printf("%s -> Integer-Literal at Position %d and Line %d\n", word, wordStart, line);
                    word[0] = '\0';
                } else if (isIdentifier(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "ID", sizeof(t.type));
                    strncpy(t.print, "Identifier", sizeof(t.print));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    // printf("%s -> Identifier at Position %d and Line %d\n", word, wordStart, line);
                    word[0] = '\0';
                } else if (isString(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "SL", sizeof(t.type));
                    strncpy(t.print, "String-Literal", sizeof(t.print));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    // printf("%s -> String-Literal at Position %d and Line %d\n", word, wordStart, line);
                    word[0] = '\0';
                } else if (isChar(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "C", sizeof(t.type));
                    strncpy(t.print, "Char-Literal", sizeof(t.print));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    // printf("%s -> Char-Literal at Position %d and Line %d\n", word, wordStart, line);
                    word[0] = '\0';
                } else if (isMultiLineComment(word)) {
                    word[0] = '\0';
                    continue;
                } else {
                    printf("Error at Position %d and line %d\n", wordStart, line);
                    word[0] = '\0';
                    break;
                }
            }
            if (isOperatorDelim(ch)) {
                append(word, ch);
                printf("Pointer: %c,Word: %s\n", ch, word);
                operatorFlag = 1;
            }
            if (isPunctuator(ch)) {
                append(tempOperator, ch);
                struct Token t;
                t.position = count;
                t.line = line;
                strncpy(t.type, "PUC", sizeof(t.type));
                strncpy(t.print, "Operator or Punctuator", sizeof(t.print));
                strncpy(t.lexem, tempOperator, sizeof(t.lexem));
                tokens[index] = t;
                index++;
                printf("%s -> Operator or Punctuator at Position %d and Line %d\n", tempOperator, count, line);
                tempOperator[0] = '\0';
            }
        } else if (ch != '\n') {
            append(word, ch);
            printf("Pointer: %c,Word: %s\n", ch, word);
        } else if (ch == '\n') {
            line++;
        }
        count++;
    }
    if (stringFlag) {
        printf("Error at Position %d no closing \" character\n", wordStart);
    }
    fclose(fp);
}

void printTable(struct Token tokens[MAX]) {
    printf("\n----------Symbol Table-----------\n");
    for (int i = 0; i < MAX; i++) {
        if (tokens[i].lexem[0] != '\0') {
            char tempword[30] = {'\0'};
            strcpy(tempword, tokens[i].print);
            for (int i = 0; i < 30; i++) {
                if (tempword[i] == '\0') {
                    tempword[i] = ' ';
                }
            }
            printf("|%s\t|\t%s\n", tempword, tokens[i].lexem);
        }
    }
}