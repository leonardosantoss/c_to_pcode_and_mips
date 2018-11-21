// Tokens
%token
  INT
  FLOAT
  VAR
  PLUS
  MINUS
  DIV
  MULT
  MOD
  GREATER
  LESS
  LESSTHAN
  GREATERTHAN
  EQUALS
  T_FLOAT
  T_INT
  T_MAIN
  T_FOR
  T_WHILE
  T_SCANF
  T_PRINTF
  T_IF
  T_ELSE
  T_OPENCURLYBRACKET
  T_CLOSECURLYBRACKET
  T_OPENPARENTESES
  T_CLOSEPARENTESES
  T_SEMICOLON
  T_COMMA
  T_AND
  T_OR
  T_NOT
  T_INCREMENT
  T_DECREMENT
  NAME
  NAME2
  EQUALSIGN
  STRING

// Operator associativity & precedence
//so pus aqui as tokens todas ainda nao tem as precedencias corretas
%left T_MAIN T_FOR T_WHILE T_SCANF T_PRINTF T_IF T_ELSE T_OPENCURLYBRACKET T_CLOSECURLYBRACKET T_OPENPARENTESES T_CLOSEPARENTESES T_SEMICOLON T_COMMA T_AND T_OR T_NOT T_INCREMENT T_DECREMENT NAME NAME2 STRING
%left GREATER GREATERTHAN LESS LESSTHAN EQUALS EQUALSIGN
%left PLUS MINUS
%left DIV MULT MOD
%left T_FLOAT T_INT
%left INT FLOAT

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  float floatValue;
  char* nameValue;
  char* name2Value;
  Expr* exprValue;
  BoolExpr* boolValue;
  BoolExprList* boolexprlistValue;
  Attrib* attribValue;
  While* whileValue;
  CmdList* cmdListValue;
  Cmd* cmdValue;
  Printf* printfValue;
  Scanf* scanfValue;
  CharList* varlistValue;
  CharList* varlist2Value;
  char* stringValue;
  If* ifValue;
  Else* elseValue;
}

%type <intValue> INT
%type <exprValue> expr
%type <boolValue> boolexpr
%type <boolexprlistValue> boolexprlist
%type <attribValue> attrib
%type <whileValue> while
%type <floatValue> FLOAT
%type <nameValue> NAME
%type <name2Value> NAME2
%type <cmdListValue> cmdList
%type <cmdValue> cmd
%type <stringValue> STRING
%type <printfValue> printf
%type <scanfValue> scanf
%type <varlistValue> varlist
%type <varlist2Value> varlist2
%type <ifValue> if
%type <elseValue> else


// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;
BoolExpr* root1;
Attrib* root2;
While* root3;

}

%%


program:
  T_INT T_MAIN T_OPENPARENTESES T_CLOSEPARENTESES T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET
  { root = $6; }
;

cmdList:
  cmd cmdList{
    $$ = ast_cmdList($1, $2);
  }
  |
  cmd
  {
    $$ = ast_cmdList($1, NULL);
  }
;

cmd:
  attrib { $$ = ast_cmd_attrib($1); }
  |
  while { $$ = ast_cmd_while($1); }
  |
  printf { $$ = ast_cmd_printf($1); }
  |
  scanf { $$ = ast_cmd_scanf($1); }
  |
  if { $$ = ast_cmd_if($1); }
;

printf:
  T_PRINTF T_OPENPARENTESES STRING varlist T_CLOSEPARENTESES T_SEMICOLON
    { $$ = ast_cmd_printf_expr($3, $4); }
  |
  T_PRINTF T_OPENPARENTESES STRING T_CLOSEPARENTESES T_SEMICOLON
    { $$ = ast_cmd_printf_expr($3, NULL); }
;

scanf:
  T_SCANF T_OPENPARENTESES STRING varlist2 T_CLOSEPARENTESES T_SEMICOLON
    { $$ = ast_cmd_scanf_expr($3, $4); }
  |
  T_SCANF T_OPENPARENTESES STRING T_CLOSEPARENTESES T_SEMICOLON
    { $$ = ast_cmd_scanf_expr($3, NULL); }
;

varlist:
  T_COMMA NAME varlist { $$ = ast_cmd_charList($2, $3); }
  |
  T_COMMA NAME { $$ = ast_cmd_charList($2, NULL); }
;

varlist2:
  T_COMMA NAME2 varlist2 { $$ = ast_cmd_charList($2, $3); }
  |
  T_COMMA NAME2 { $$ = ast_cmd_charList($2, NULL); }
;

while:
  T_WHILE T_OPENPARENTESES boolexprlist T_CLOSEPARENTESES T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET{
    $$ = ast_cmd_while_boolexpr($3, $6);
  }
  |
  T_WHILE T_OPENPARENTESES expr T_CLOSEPARENTESES T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET{
    $$ = ast_cmd_while_expr($3, $6);
  }
;

if:
  T_IF T_OPENPARENTESES boolexprlist T_CLOSEPARENTESES T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET{
    $$ = ast_cmd_if_boolexpr($3, $6);
  }
  |
  T_IF T_OPENPARENTESES expr T_CLOSEPARENTESES T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET{
    $$ = ast_cmd_if_expr($3, $6);
  }
  |
  T_IF T_OPENPARENTESES boolexprlist T_CLOSEPARENTESES T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET else{
    $$ = ast_cmd_ifelse_boolexpr($3, $6, $8);
  }
  |
  T_IF T_OPENPARENTESES expr T_CLOSEPARENTESES T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET else{
    $$ = ast_cmd_ifelse_expr($3, $6, $8);
  }
;

else:
  T_ELSE T_OPENCURLYBRACKET cmdList T_CLOSECURLYBRACKET{
    $$ = ast_cmd_else_expr($3);
  }
;

attrib:
  T_INT NAME EQUALSIGN expr T_SEMICOLON{
    $$ = ast_attrib_expr_ct($2, $4);
  }
  |
  NAME EQUALSIGN expr T_SEMICOLON{
    $$ = ast_attrib_expr($1, $3);
  }
  |
  T_INT NAME T_SEMICOLON{
      $$ = ast_non_attrib($2);
  }
;

boolexpr:
  expr GREATERTHAN expr {
    $$ = ast_boolean_expr(GREATERTHAN, $1, $3);
  }
  |
   expr LESSTHAN expr {
    $$ = ast_boolean_expr(LESSTHAN, $1, $3);
  }
  |
   expr GREATER expr {
    $$ = ast_boolean_expr(GREATER, $1, $3);
  }
  |
   expr LESS expr {
    $$ = ast_boolean_expr(LESS, $1, $3);
  }
  |
   expr EQUALS expr {
    $$ = ast_boolean_expr(EQUALS, $1, $3);
  }
;

boolexprlist:
  boolexpr T_AND boolexprlist
  {
    $$ = ast_boolean_exprList_and($1, $3);
  }
  |
  boolexpr T_OR boolexprlist
  {
    $$ = ast_boolean_exprList_or($1, $3);
  }
  |
  boolexpr
  {
    $$ = ast_boolean_exprList_solo($1, NULL);
  }
  |
  expr T_AND boolexprlist
  {
    $$ = ast_expr_exprList_and($1, $3);
  }
  |
  expr T_OR boolexprlist
  {
    $$ = ast_expr_exprList_or($1, $3);
  }
  |
  expr 
  {
    $$ = ast_expr_exprList_solo($1, NULL);
  }
;

expr:
  INT {
    $$ = ast_integer($1);
  }
  |
  NAME {
    $$ = ast_expr_var($1);
  }
  |
  expr PLUS expr {
    $$ = ast_operation(PLUS, $1, $3);
  }
  |
  expr MINUS expr {
    $$ = ast_operation(MINUS, $1, $3);
  }
  |
  expr DIV expr {
    $$ = ast_operation(DIV, $1, $3);
  }
  |
  expr MULT expr {
    $$ = ast_operation(MULT, $1, $3);
  }
  |
  expr MOD expr {
    $$ = ast_operation(MOD, $1, $3);
  }

  ;

%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}
