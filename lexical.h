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
//Checks if Word belongs to the operator array
int isOperatorDelim(char str) {
    char word[MAX] = "";
    append(word, str);
    for (int i = 0; i < LEN(operators); i++) {
        if (strcmp(word, operators[i]) == 0) {
            word[0] = '\0';
            return 1;
        }
    }
    word[0] = '\0';
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
    if (regcomp(&re, "^[0-9]+", REG_EXTENDED | REG_NOSUB) != 0)
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
    if (regcomp(&re, "^(([0-9]+)?\\.[0-9]*)", REG_EXTENDED | REG_NOSUB) != 0)
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
    if (regcomp(&re, "^@?[_a-zA-Z][-_a-zA-Z0-9]*", REG_EXTENDED | REG_NOSUB) != 0)
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
//Checks if string is single line comment
int isSingleLineComment(char word[]) {
    regex_t re;
    if (regcomp(&re, "\\/\\/.*", REG_EXTENDED | REG_NOSUB) != 0)
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
    //Flags for specific cases
    int stringFlag = 0;
    int operatorFlag = 0;
    int numericFlag = 0;
    int singleFlag = 0;
    int multiFlag = 0;

    int ch;                      //Character
    int count = 1;               //Char Position
    int wordStart = 1;           //Word Position
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
                strncpy(t.type, "Operator or Punctuator", sizeof(t.type));
                strncpy(t.lexem, word, sizeof(t.lexem));
                tokens[index] = t;
                index++;
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
        }
        if (strcmp(word, "/*") == 0) {
            multiFlag = 1;
        }
        if (ch == '/' && multiFlag == 1) {
            append(word, ch);
            multiFlag = 0;
        } else {
            multiFlag = 0;
        }
        if (ch == '\"') {
            stringFlag = stringFlag == 0 ? 1 : 0;
        }
        /*Word Processing*/
        if ((ch == ' ' || isPunctuator(ch) || isOperatorDelim(ch)) &&
            (numericFlag != 1 && stringFlag != 1 && singleFlag != 1 && multiFlag != 1 && operatorFlag != 1)) {
            if (word[0] != '\0') {
                if (isKeyword(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "Keyword", sizeof(t.type));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    word[0] = '\0';
                } else if (isDouble(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "Real-Literal", sizeof(t.type));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    word[0] = '\0';
                } else if (isNumber(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "Integer-Literal", sizeof(t.type));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    word[0] = '\0';
                } else if (isIdentifier(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "Identifier", sizeof(t.type));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    word[0] = '\0';
                } else if (isString(word)) {
                    struct Token t;
                    t.position = wordStart;
                    t.line = line;
                    strncpy(t.type, "String-Literal", sizeof(t.type));
                    strncpy(t.lexem, word, sizeof(t.lexem));
                    tokens[index] = t;
                    index++;
                    word[0] = '\0';
                } else if (isMultiLineComment(word)) {
                    word[0] = '\0';
                } else {
                    printf("Error at Position %d and line %d\n", wordStart, line);
                    word[0] = '\0';
                    break;
                }
            }
            if (isOperatorDelim(ch)) {
                append(word, ch);
                operatorFlag = 1;
            }
            if (isPunctuator(ch)) {
                append(tempOperator, ch);
                struct Token t;
                t.position = count;
                t.line = line;
                strncpy(t.type, "Operator or Punctuator", sizeof(t.type));
                strncpy(t.lexem, tempOperator, sizeof(t.lexem));
                tokens[index] = t;
                index++;
                tempOperator[0] = '\0';
            }
        } else if (ch != '\n') {
            append(word, ch);
        } else if (ch == '\n') {
            line++;
        }
        count++;
    }
    if (stringFlag) {
        printf("Error at Position %d no clossing \" character\n", wordStart);
    }
    fclose(fp);
}
