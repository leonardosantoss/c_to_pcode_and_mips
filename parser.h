/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 98 "parser.bison" /* yacc.c:1909  */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;
Expr* root0;
BoolExpr* root1;
Attrib* root2;
While* root3;
Scanf* root4;
Printf* root5;

#line 63 "parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    VAR = 260,
    PLUS = 261,
    MINUS = 262,
    DIV = 263,
    MULT = 264,
    MOD = 265,
    GREATER = 266,
    LESS = 267,
    LESSTHAN = 268,
    GREATERTHAN = 269,
    EQUALS = 270,
    NEQUALS = 271,
    T_FLOAT = 272,
    T_INT = 273,
    T_MAIN = 274,
    T_FOR = 275,
    T_WHILE = 276,
    T_SCANF = 277,
    T_PRINTF = 278,
    T_IF = 279,
    T_ELSE = 280,
    T_OPENCURLYBRACKET = 281,
    T_CLOSECURLYBRACKET = 282,
    T_OPENPARENTESES = 283,
    T_CLOSEPARENTESES = 284,
    T_SEMICOLON = 285,
    T_COMMA = 286,
    T_AND = 287,
    T_OR = 288,
    T_NOT = 289,
    T_INCREMENT = 290,
    T_DECREMENT = 291,
    NAME = 292,
    NAME2 = 293,
    EQUALSIGN = 294,
    STRING = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 55 "parser.bison" /* yacc.c:1909  */

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

#line 137 "parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
