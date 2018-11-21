#include <stdio.h>
#include "parser.h"
#include "stack.h"

void printInstr(Instr* instr){
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

void printInstrList(InstrList* root){
    if(root!=NULL){
      printInstr(root->instr);
      printInstrList(root->next);
      
    }
}

InstrList* compile(Expr* expr){
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

  InstrList* instrlist = compile(expr->attr.op.left);
  stack_instrlist_append(instrlist, compile(expr->attr.op.right));
  stack_instrlist_append(instrlist, stack_instrlist(tmp, NULL));  


  return instrlist;
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

    
    InstrList* instr = compile(root0);
    printInstrList(instr);
  }
  return 0;


}
