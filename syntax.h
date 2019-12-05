#include "mpc.h"
#define MAX 2048
void appendString(char str1[], char str2[])
{
    strcat(str1, str2);
}
mpc_result_t makeAST(struct Token tokens[])
{
    mpc_parser_t *compilation_unit = mpc_new("compilation_unit");
    mpc_parser_t *namespace_declaration = mpc_new("namespace_declaration");
    mpc_parser_t *class_declaration = mpc_new("class_declaration");
    mpc_parser_t *main_method = mpc_new("main_method");
    mpc_parser_t *class_modifier = mpc_new("class_modifier");
    mpc_parser_t *statement = mpc_new("statement");
    mpc_parser_t *flow_alt = mpc_new("flow_alt");
    mpc_parser_t *flow_structure_body = mpc_new("flow_structure_body");
    mpc_parser_t *if_statement = mpc_new("if_statement");
    mpc_parser_t *else_clause = mpc_new("else_clause");
    mpc_parser_t *while_statement = mpc_new("while_statement");
    mpc_parser_t *do_while_statement = mpc_new("do_while_statement");
    mpc_parser_t *for_statement = mpc_new("for_statement");
    mpc_parser_t *iterator_section = mpc_new("iterator_section");
    mpc_parser_t *var_type = mpc_new("var_type");
    mpc_parser_t *id_or_assignment = mpc_new("id_or_assignment");
    mpc_parser_t *declaration_statement = mpc_new("declaration_statement");
    mpc_parser_t *assignment_statement = mpc_new("assignment_statement");
    mpc_parser_t *predefined_type = mpc_new("predefined_type");
    mpc_parser_t *type = mpc_new("type");
    mpc_parser_t *expression = mpc_new("expression");
    mpc_parser_t *cast_expression = mpc_new("cast_expression");
    mpc_parser_t *post_op_expression = mpc_new("post_op_expression");
    mpc_parser_t *boolean_expression = mpc_new("boolean_expression");
    mpc_parser_t *array_creation_expression = mpc_new("array_creation_expression");
    mpc_parser_t *unary_op_expression = mpc_new("unary_op_expression");
    mpc_parser_t *binary_op_alt = mpc_new("binary_op_alt");
    mpc_parser_t *binary_op_expression = mpc_new("binary_op_expression");
    mpc_parser_t *size_of_expression = mpc_new("size_of_expression");
    mpc_parser_t *literal_expression = mpc_new("literal_expression");
    mpc_parser_t *binary_operator = mpc_new("binary_operator");
    mpc_parser_t *array_indexing_expression = mpc_new("array_indexing_expression");
    mpc_parser_t *unary_operator = mpc_new("unary_operator");
    mpc_parser_t *string_literal = mpc_new("string_literal");
    mpc_parser_t *bool_literal = mpc_new("bool_literal");
    mpc_parser_t *real_literal = mpc_new("real_literal");
    mpc_parser_t *integer_literal = mpc_new("integer_literal");
    mpc_parser_t *char_literal = mpc_new("char_literal");
    mpc_parser_t *identifier = mpc_new("identifier");
    mpc_parser_t *expression_statement = mpc_new("expression_statement");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                     \
compilation_unit : /^/ <namespace_declaration> /$/ ;  \
namespace_declaration : \"namespace\" <identifier> '{' <class_declaration> '}'; \
class_declaration : (<class_modifier>)* \"class\" <identifier> '{' <main_method> '}'; \
main_method : (\"public\")? \"static\" \"void\" \"Main\" '(' (\"string\" '[' ']' \"args\")? ')' '{' (<statement>)* '}' ; \
class_modifier: \"public\" | \"protected\" | \"private\" | \"abstract\"; \
statement: <if_statement> | <while_statement> | <do_while_statement> | <for_statement> | <declaration_statement> | <assignment_statement> | <expression_statement> ; \
flow_alt : '{' (<statement>)* '}'; \
expression_statement: <expression> ';' ; \
flow_structure_body : <flow_alt> | <statement> ; \
if_statement : \"if\" '(' <boolean_expression> ')' <flow_structure_body> (<else_clause>)? ; \
else_clause : \"else\" <flow_structure_body> ; \
while_statement : \"while\" '(' <boolean_expression> ')' <flow_structure_body> ; \
do_while_statement : \"do\" '{' (<statement>)* '}' \"while\" '(' <boolean_expression> ')' ';' ; \
for_statement : \"for\" '(' <declaration_statement> <boolean_expression> ';' <iterator_section> ')' <flow_structure_body> ; \
iterator_section : <identifier> ( \"++\" | \"--\" ) ;\
var_type: <type> | \"var\" ; \
id_or_assignment:  <identifier> ';' | <assignment_statement> ; \
declaration_statement: <var_type> <id_or_assignment> ; \
assignment_statement:  <identifier> ( '[' <expression> ']')? '=' <expression> ';' ; \
predefined_type: \"bool\" | \"byte\" | \"char\"  | \"double\" | \"float\" | \"int\" | \"long\" | \"sbyte\" | \"short\" | \"string\" | \"uint\" | \"ulong\" | \"ushort\" ;\
type: <predefined_type> ('[' ']')? ; \
expression : <unary_op_expression> | <binary_op_expression> | <post_op_expression> | <cast_expression> | '(' <expression> ')' | <size_of_expression> | <array_creation_expression> | <literal_expression> | <array_indexing_expression> | <identifier> ; \
cast_expression : '(' <predefined_type> ')' <expression> ; \
post_op_expression : <binary_op_alt> ( \"++\" | \"--\" )  ;  \
boolean_expression : <expression> ; \
array_creation_expression : \"new\" <predefined_type> '[' <integer_literal> ']' ; \
unary_op_expression : <unary_operator> <expression> ; \
binary_op_alt : <literal_expression> | <array_indexing_expression> | <identifier> ; \
binary_op_expression : <binary_op_alt> <binary_operator> <expression> ; \
size_of_expression : \"sizeof\" '(' <type> ')' ; \
literal_expression : <string_literal> | <bool_literal> | <real_literal> | <integer_literal> | <char_literal>  ; \
binary_operator : \">>\"| \"<<\" | \"<=\" | \"==\" | \">=\" | \"!=\"| '+' | '*' | '/' | '%' | '-' | '<' | '>' ; \
array_indexing_expression : <identifier> '[' <expression> ']' ; \
unary_operator :   \"++\" | \"--\" | '!' | '~' | '+' | '-' ; \
string_literal : '\"' /[ a-zA-Z0-9_]*/ '\"' ; \
bool_literal : \"true\" | \"false\"  ; \
real_literal : (<integer_literal>)? '.' <integer_literal> ; \
integer_literal : /[0-9]+/ ; \
char_literal : /'.'/ ; \
identifier : /@?[_a-zA-Z][_a-zA-Z0-9]*/ ; \
",
              compilation_unit, namespace_declaration, class_declaration, main_method, class_modifier, statement,
              flow_structure_body, if_statement, else_clause, while_statement, do_while_statement, for_statement,
              iterator_section, assignment_statement, declaration_statement, predefined_type, type, expression, post_op_expression,
              boolean_expression, array_creation_expression, unary_op_expression, binary_op_expression, size_of_expression,
              literal_expression, binary_operator, array_indexing_expression, unary_operator, string_literal, bool_literal,
              real_literal, integer_literal, char_literal, identifier, expression_statement, var_type, flow_alt, id_or_assignment,
              binary_op_alt, cast_expression);
    mpc_result_t r;
    char program[MAX * 2];
    for (int i = 0; i < MAX; i++)
    {
        if (tokens[i].lexem[0] != '\0')
        {
            appendString(program, tokens[i].lexem);
            appendString(program, " ");
        }
    }
    if (mpc_parse("<stdin>", program, compilation_unit, &r))
    {
        /* On Success Print the AST */
        printf("\n");
        mpc_ast_print(r.output);
        printf("\n");
    }
    else
    {
        /* Otherwise Print the Error */
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }
    mpc_cleanup(40, compilation_unit, namespace_declaration, class_declaration, main_method, class_modifier, statement,
                flow_structure_body, if_statement, else_clause, while_statement, do_while_statement, for_statement,
                iterator_section, assignment_statement, declaration_statement, predefined_type, type, expression, post_op_expression,
                boolean_expression, array_creation_expression, unary_op_expression, binary_op_expression, size_of_expression,
                literal_expression, binary_operator, array_indexing_expression, unary_operator, string_literal, bool_literal,
                real_literal, integer_literal, char_literal, identifier, expression_statement, var_type, flow_alt, id_or_assignment,
                binary_op_alt, cast_expression);
    return r;
}