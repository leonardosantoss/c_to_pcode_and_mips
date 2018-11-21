%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++;}


\-?[0-9]+ {
   yylval.intValue = atoi(yytext);
   return INT;
}

"+" { return PLUS; }
"-" { return MINUS; }
"/" { return DIV;}
"%" { return MOD; }
"*" { return MULT; }
">" { return GREATER; }
"<" { return LESS; }
">=" { return GREATERTHAN; }
"<=" { return LESSTHAN; }
"==" { return EQUALS; }
"int" { return T_INT; }
"float" { return T_FLOAT; }
"=" { return EQUALSIGN; }
"main" { return T_MAIN; }
"for" { return T_FOR; }
"while" { return T_WHILE; }
"scanf" { return T_SCANF; }
"printf" { return T_PRINTF; }
"if" { return T_IF; }
"else" { return T_ELSE; }
"{" { return T_OPENCURLYBRACKET; }
"}" { return T_CLOSECURLYBRACKET; }
"(" { return T_OPENPARENTESES; }
")" { return T_CLOSEPARENTESES; }
";" { return T_SEMICOLON; }
"," { return T_COMMA; }
"&&" { return T_AND; }
"||" { return T_OR; }
"!" { return T_NOT; }
"++" { return T_INCREMENT; }
"--" { return T_DECREMENT; }

[a-z][a-zA-Z0-9]* {
    yylval.nameValue = strdup(yytext);
    return NAME;
}

\&[a-z][a-zA-Z0-9]* {
    yylval.name2Value = strdup(yytext);
    return NAME2;
}

\"[^\"]*\" {
    yylval.stringValue = strdup(yytext);
    return STRING;
}
.  { yyerror("unexpected character"); }
%%
