#include <stdio.h>
#include "parser.h"
#include "stack.h"
#include "ast.h"

void printInstrExpr(Instr* instr){
    switch(instr->type){
      case E_LDC:
        printf("ldc %d\n", instr->attr.value);
        break;
      case E_ADI:
        printf("adi\n");
        break;
      case E_MPI:
        printf("mpi\n");
        break;
      case E_SBI:
        printf("sbi\n");
        break;
      default:
        break;  
    }
}

void printInstrScanf(Instr* instr)
{
  switch(instr->type)
  {
    case E_LDA:
      printf("lda %s\n", instr->attr.name);
      break;
    case E_RDI:
      printf("rdi\n");
      break;
    default:
      break;
  }
}

void printInstrPrintf(Instr* instr){
  switch(instr->type){
    case E_WRI:
      printf("wri\n");
      break;
    case E_LOD:  
      printf("lod %s\n", instr->attr.name);
      break;
    default:
      break;
  }
}

void printInstrExprList(InstrList* root){
    if(root!=NULL){
      printInstrExpr(root->instr);
      printInstrExprList(root->next);
    }
}

void printInstrScanfList(InstrList* root){
  if(root != NULL)
  {
    printInstrScanf(root->instr);
    printInstrScanfList(root->next);
  }
}

void printInstrPrintfList(InstrList* root){
  if(root != NULL)
  {
    printInstrPrintf(root->instr);
    printInstrPrintfList(root->next);
  }
}

InstrList* compileExpr(Expr* expr){
  if(expr->kind == E_INTEGER){
    return stack_instrlist(stack_instr_ldc(expr->attr.value), NULL);
  }

  Instr* tmp;
  switch(expr->attr.op.operator){
    
    case PLUS:
      tmp =stack_instr_adi();
      break;
    case MINUS:
      tmp =stack_instr_sbi();
      break;
    case MULT:
      tmp = stack_instr_mpi();
      break;
  }

  InstrList* instrlist = compileExpr(expr->attr.op.left);
  stack_instrlist_append(instrlist, compileExpr(expr->attr.op.right));
  stack_instrlist_append(instrlist, stack_instrlist(tmp, NULL));  


  return instrlist;
}

InstrList* compileScanf(CharList* charlist){
    InstrList* instrlist1 = stack_instrlist( stack_instr_lda(charlist->value), NULL);
    InstrList* instrlist2 = stack_instrlist( stack_instr_rdi(), NULL);
    stack_instrlist_append( instrlist1, instrlist2);
    
    if(charlist->next != NULL)
      stack_instrlist_append(instrlist1, compileScanf(charlist->next));
    return instrlist1;
}

InstrList* compilePrintf(CharList* charlist){
    InstrList* instrlist1 = stack_instrlist( stack_instr_lod(charlist->value), NULL);
    InstrList* instrlist2 = stack_instrlist( stack_instr_wri(), NULL);
    stack_instrlist_append( instrlist1, instrlist2);
    
    if(charlist->next != NULL)
      stack_instrlist_append(instrlist1, compilePrintf(charlist->next));
    return instrlist1;
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

    
    //InstrList* instr = compileExpr(root0);
    //printInstrExprList(instr);

    InstrList* instr = compilePrintf(root5->varList);
    printInstrPrintfList(instr);
  }
  return 0;


}
