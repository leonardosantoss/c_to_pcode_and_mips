#include <stdio.h>
#include "parser.h"

void printCmdList(CmdList* root, int nSpaces);

void print_aux(int nSpaces){
  int i;
  for(i=0;i<nSpaces;i++){
    printf("  ");
  }
}

void printCharList(CharList* varlist, int nSpaces){
  print_aux(nSpaces);
  printf(" %s\n", varlist->value);

  if(varlist->next != NULL){
    printCharList(varlist->next, nSpaces);
  }
}

void printPrintf(Printf* Printf, int nSpaces){
  print_aux(nSpaces);
  printf("printf\n");
  print_aux(nSpaces+2);
  printf("%s\n",  Printf->value);
  if(Printf->varList != NULL){
    printCharList(Printf->varList, nSpaces+2);
  }
}

void printScanf(Scanf* Scanf, int nSpaces){
  print_aux(nSpaces);
  printf("scanf\n");
  print_aux(nSpaces+2);
  printf("%s\n",  Scanf->value);
  if(Scanf->varList != NULL){
    printCharList(Scanf->varList, nSpaces+2);
  }
}

void printExpr(Expr* expr, int nSpaces){

  if(expr->kind == E_INTEGER){
    print_aux(nSpaces);
    printf("%d\n", expr->attr.value);
  }
  else if(expr->kind == E_VAR){
    print_aux(nSpaces);
    printf("%s\n", expr->attr.var);

  }
  else if(expr->kind == E_OPERATION){

    print_aux(nSpaces);
    switch(expr->attr.op.operator){
      case PLUS:
        printf("+\n");
        break;
      case MINUS:
        printf("-\n");
        break;
      case MULT:
        printf("*\n");
        break;
      case DIV:
        printf("/\n");
        break;
      case MOD:
        printf("%c\n", '%');
        break;

    }

    printExpr(expr->attr.op.left, nSpaces+2);
    printExpr(expr->attr.op.right, nSpaces+2);
  }

}

void printVar(Expr* value, char* name, int nSpaces){
  print_aux(nSpaces);
  printf("%s\n", name);
  print_aux(nSpaces+2);
  printf("=\n");
  printExpr(value, nSpaces+2);
}

void printAttrib(Attrib* attrib, int nSpaces){
  if(attrib->kind == E_ATTRIBCT){
    print_aux(nSpaces);
    printf("int\n");
    printVar(attrib->attr.attct.value, attrib->attr.attct.name, nSpaces+2);
  }
  else if(attrib->kind == E_ATTRIBST)
  {
     printVar(attrib->attr.att.value, attrib->attr.att.name, nSpaces);
  }
  else if(attrib->kind == E_NONATTRIB){
    print_aux(nSpaces);
    printf("int\n");
    print_aux(nSpaces+2);
    printf("%s\n", attrib->attr.name);
  }
}


void printBoolExpr(BoolExpr* boolExpr, int nSpaces){

  if(boolExpr->kind == E_BOOL){
    printExpr(boolExpr->attr.value, nSpaces);
  }
  else if(boolExpr->kind == E_BOOLOPERATION){
    print_aux(nSpaces);
    switch(boolExpr->attr.op.operator){
      case GREATER:
        printf(">\n");
        break;
      case LESS:
        printf("<\n");
        break;
      case GREATERTHAN:
        printf(">=\n");
        break;
      case LESSTHAN:
        printf("<=\n");
        break;
      case EQUALS:
        printf("==\n");
        break;

    }
    printExpr(boolExpr->attr.op.left, nSpaces+2);
    printExpr(boolExpr->attr.op.right, nSpaces+2);
  }

}

void printBoolExprList(BoolExprList* root, int nSpaces)
{
  if(root->kind == E_EXPR){
    printExpr(root->list.type.expr, nSpaces);
  }
  else if(root->kind == E_BOOLEXPR){
    printBoolExpr(root->list.type.value, nSpaces);
  }
  else if(root->kind == E_EXPRAND){
    printExpr(root->list.type.expr, nSpaces);
    print_aux(nSpaces);
    printf("&&\n");
  }
  else if(root->kind == E_EXPROR){
    printExpr(root->list.type.expr, nSpaces);
    print_aux(nSpaces);
    printf("||\n");
  }
  else if(root->kind == E_AND){
    printBoolExpr(root->list.type.value, nSpaces);
    print_aux(nSpaces);
    printf("&&\n");
  }
  else if(root->kind == E_OR){
    printBoolExpr(root->list.type.value, nSpaces);
    print_aux(nSpaces);
    printf("||\n");
  }
  if(root->list.next != NULL)
  {
    root = root->list.next;
    printBoolExprList(root, nSpaces);
  }
}

void printWhile(While* cmdWhile, int nSpaces){
    if(cmdWhile->kind == E_WHILE_BOOLEXPR){
      print_aux(nSpaces);
      printf("while\n");
      printBoolExprList(cmdWhile->type.valueBoolExpr, nSpaces+2);
      printCmdList(cmdWhile->test ,nSpaces+2);
    }
    else if(cmdWhile->kind == E_WHILE_EXPR){
      print_aux(nSpaces);
      printf("while\n");
      printExpr(cmdWhile->type.valueExpr, nSpaces+2);
      printCmdList(cmdWhile->test ,nSpaces+2);
    }
}

void printElse(Else* cmdElse, int nSpaces)
{
    print_aux(nSpaces);
    printf("else\n");
    printCmdList(cmdElse->test ,nSpaces+2);
}

void printIf(If* cmdIf, int nSpaces){
  if(cmdIf->kind == E_IF_BOOLEXPR){
      print_aux(nSpaces);
      printf("if\n");
      printBoolExprList(cmdIf->type.valueBoolExpr, nSpaces+2);
      printCmdList(cmdIf->test ,nSpaces+4);
    }
  else if(cmdIf->kind == E_IF_EXPR){
    print_aux(nSpaces);
    printf("if\n");
    printExpr(cmdIf->type.valueExpr, nSpaces+2);
    printCmdList(cmdIf->test ,nSpaces+4);
  }
  else if(cmdIf->kind == E_IFELSE_BOOLEXPR){
    print_aux(nSpaces);
    printf("if\n");
    printBoolExprList(cmdIf->type.valueBoolExpr, nSpaces+2);
    printCmdList(cmdIf->test ,nSpaces+4);
    printElse(cmdIf->withElse.valueElse ,nSpaces);
  }
  else if(cmdIf->kind == E_IFELSE_EXPR){
    print_aux(nSpaces);
    printf("if\n");
    printExpr(cmdIf->type.valueExpr, nSpaces+2);
    printCmdList(cmdIf->test ,nSpaces+4);
    printElse(cmdIf->withElse.valueElse ,nSpaces);
  }
}


void printCmd(Cmd* cmd, int nSpaces)
{
  if(cmd->kind == E_ATTRIB)
  {
    //print_aux(nSpaces);
    printAttrib(cmd->type.Attrib, nSpaces+2);
  }
  else if(cmd->kind == E_WHILE)
  {
    //print_aux(nSpaces);
    printWhile(cmd->type.While, nSpaces+2);
  }
  else if(cmd->kind == E_PRINTF)
  {
    //print_aux(nSpaces);
    printPrintf(cmd->type.Printf, nSpaces+2);
  }
  else if (cmd->kind == E_SCANF){
    //print_aux(nSpaces);
    printScanf(cmd->type.Scanf, nSpaces+2);
  }
  else if(cmd->kind == E_IF){
    //print_aux(nSpaces);
    printIf(cmd->type.If, nSpaces+2);
  }
}

void printCmdList(CmdList* root, int nSpaces){
    printCmd(root->Cmd, nSpaces);

    while(root->next != NULL)
    {
      root = root->next;
      printCmd(root->Cmd, nSpaces);
    }
}

int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  if (yyparse() == 0) {

    //printBoolExpr(root, 0);
    //printAttrib(root2, 0);
    //printWhile(root3, 0);

    printf("int main()\n");
    printCmd(root->Cmd, 0);

    while(root->next != NULL)
    {
      root = root->next;
      printCmd(root->Cmd, 0);
    }

  }
  return 0;


}
