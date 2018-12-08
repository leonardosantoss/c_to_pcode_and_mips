#include <stdio.h>
#include "parser.h"
#include "stack.h"
#include "ast.h"

#define VARNAME instrlist->instr->attr

int labelGlobal = 0;

InstrList* compileCmdList(CmdList* cmdlist);
InstrList* compileCmd(Cmd* cmd);

void printInstrCmd(Instr* instr)
{
  switch(instr->type){
    case E_LDC:
      printf("ldc %d\n", instr->attr.value);
      break;
    case E_LOD:
        printf("lod %s\n", instr->attr.name);
      break;
    case E_LDA:
      printf("lda %s\n", instr->attr.name);
      break;
    case E_INT:
      printf("var %s\n", instr->attr.name);
      break;
    case E_WRI:
      printf("wri\n");
      break;
    case E_RDI:
      printf("rdi\n");
      break;
    case E_ADI:
      printf("adi\n");
      break;
    case E_SBI:
      printf("sbi\n");
      break;
    case E_MPI:
      printf("mpi\n");
      break;
    case E_DVI:
      printf("dvi\n");
      break;
    case E_MOD:
      printf("mod\n");
      break;
    case E_STO:
      printf("sto\n");
      break;
    case E_FJP:
      printf("fjp %s\n", instr->attr.name);
      break;
    case E_UJP:
      printf("ujp %s\n", instr->attr.name);
      break;
    case E_LAB:
      printf("%s\n", instr->attr.name);
      break;
    case E_GEQ:
      printf("geq\n");
      break;
    case E_GES:
      printf("ges\n");
      break;
    case E_EQU:
      printf("equ\n");
      break;
    case E_LEQ:
      printf("leq\n");
      break;
    case E_LES:
      printf("les\n");
      break;
    default:
      break;
  }
}

void printInstrCmdList(InstrList* root){
    printInstrCmd(root->instr);
    while(root->next != NULL)
    {
      root = root->next;
      printInstrCmd(root->instr);
    }
}

InstrList* compileExpr(Expr* expr){
  if(expr->kind == E_INTEGER){
    return stack_instrlist(stack_instr_ldc(expr->attr.value), NULL);
  }
  if(expr->kind == E_VAR)
  {
    return stack_instrlist(stack_instr_lod(expr->attr.var), NULL);
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

InstrList* compileBoolExpr(BoolExpr* boolexpr){
  Instr* tmp;
  if(boolexpr->kind == E_BOOL){
    return compileExpr(boolexpr->attr.value);
  }
  if(boolexpr->kind == E_BOOLOPERATION){
    switch(boolexpr->attr.op.operator){
      case GREATER:
        tmp =stack_instr_ges();
        break;
      case GREATERTHAN:
        tmp =stack_instr_geq();
        break;
      case LESSTHAN:
        tmp = stack_instr_leq();
        break;
      case LESS:
        tmp = stack_instr_les();
        break;
      case EQUALS:
        tmp = stack_instr_equ();
      default:
        break;
   }
  }

  InstrList* instrlist = compileExpr(boolexpr->attr.op.left);
  stack_instrlist_append(instrlist, compileExpr(boolexpr->attr.op.right));
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

InstrList* compileAttrib(Attrib* attrib){
    InstrList* instrlist1;
    switch(attrib->kind){
      case E_ATTRIBST:
        instrlist1 = stack_instrlist(stack_instr_lda(attrib->attr.att.name), NULL);
        stack_instrlist_append( instrlist1, compileExpr(attrib->attr.att.value));
        stack_instrlist_append( instrlist1, stack_instrlist(stack_instr_sto(), NULL));
        break;
      case E_ATTRIBCT:
        instrlist1 = stack_instrlist(stack_instr_var(attrib->attr.attct.name), NULL);
        stack_instrlist_append( instrlist1, stack_instrlist(stack_instr_lda(attrib->attr.attct.name), NULL));
        stack_instrlist_append( instrlist1, compileExpr(attrib->attr.attct.value));
        stack_instrlist_append( instrlist1, stack_instrlist(stack_instr_sto(), NULL));
        break;
      case E_NONATTRIB:
        instrlist1 = stack_instrlist(stack_instr_var(attrib->attr.name), NULL);
        stack_instrlist_append( instrlist1, stack_instrlist(stack_instr_lda(attrib->attr.name), NULL));
        break;
      default:
        break;
    }
    return instrlist1;
}

InstrList* compileWhile(While* whilecmd)
{
  labelGlobal++;
  int labelLocal = labelGlobal;
  InstrList* instrlist1;

  instrlist1 = stack_instrlist(stack_instr_label(labelLocal), NULL);

  stack_instrlist_append(instrlist1, compileCmdList(whilecmd->test));

  switch(whilecmd->kind){
    case E_WHILE_EXPR:
      stack_instrlist_append(instrlist1, compileExpr(whilecmd->type.valueExpr));
      break;
    case E_WHILE_BOOLEXPR:
      stack_instrlist_append(instrlist1, compileBoolExpr(whilecmd->type.valueBoolExpr->list.type.value));
      break;
    default:
      break;
  }

  stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_fjp(labelLocal), NULL));
  return instrlist1;
}

InstrList* compileIf(If* ifcmd)
{
  int labelLocal1, labelLocal2;
  InstrList* instrlist1;
  switch(ifcmd->kind){
    case E_IF_EXPR:
      labelGlobal++;
      labelLocal1 = labelGlobal;
      instrlist1 = compileExpr(ifcmd->type.valueExpr);
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_fjp(labelLocal1), NULL));
      stack_instrlist_append(instrlist1, compileCmdList(ifcmd->test));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_label(labelLocal1), NULL));
      break;
    case E_IF_BOOLEXPR:
      labelGlobal++;
      labelLocal1 = labelGlobal;
      instrlist1 = compileBoolExpr(ifcmd->type.valueBoolExpr->list.type.value);
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_fjp(labelLocal1), NULL));
      stack_instrlist_append(instrlist1, compileCmdList(ifcmd->test));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_label(labelLocal1), NULL));
      break;
    case E_IFELSE_EXPR:
      labelGlobal++;
      labelLocal1 = labelGlobal;
      labelGlobal++;
      labelLocal2 = labelGlobal;

      instrlist1 = compileExpr(ifcmd->type.valueExpr);
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_fjp(labelLocal1), NULL));
      stack_instrlist_append(instrlist1, compileCmdList(ifcmd->test));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_ujp(labelLocal2), NULL));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_label(labelLocal1), NULL));
      stack_instrlist_append(instrlist1, compileCmdList(ifcmd->withElse.valueElse->test));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_label(labelLocal2), NULL));
      break;
    case E_IFELSE_BOOLEXPR:
      labelGlobal++;
      labelLocal1 = labelGlobal;
      labelGlobal++;
      labelLocal2 = labelGlobal;

      instrlist1 = compileBoolExpr(ifcmd->type.valueBoolExpr->list.type.value);
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_fjp(labelLocal1), NULL));
      stack_instrlist_append(instrlist1, compileCmdList(ifcmd->test));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_ujp(labelLocal2), NULL));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_label(labelLocal1), NULL));
      stack_instrlist_append(instrlist1, compileCmdList(ifcmd->withElse.valueElse->test));
      stack_instrlist_append(instrlist1, stack_instrlist(stack_instr_label(labelLocal2), NULL));
      break;
    default:
      break;
  }

  return instrlist1;
}

InstrList* compileCmd(Cmd* cmd){
  InstrList* instrlist1;
  switch(cmd->kind){
    case E_PRINTF:
      instrlist1 = compilePrintf(cmd->type.Printf->varList);
      break;
    case E_SCANF:
      instrlist1 = compileScanf(cmd->type.Scanf->varList);
      break;
    case E_ATTRIB:
      instrlist1 = compileAttrib(cmd->type.Attrib);
      break;
    case E_WHILE:
      instrlist1 = compileWhile(cmd->type.While);
      break;
    case E_IF:
      instrlist1 = compileIf(cmd->type.If);
    default:
      break;
  }
  return instrlist1;
}

InstrList* compileCmdList(CmdList* cmdlist){
  InstrList* instrlist1;
  instrlist1 = compileCmd(cmdlist->Cmd);

  while(cmdlist->next != NULL){
    cmdlist = cmdlist->next;
    stack_instrlist_append(instrlist1, compileCmd(cmdlist->Cmd));
  }

  return instrlist1;
}

void printData(InstrList* instrlist){

    printf(".data\n");

    while(instrlist->next != NULL)
    {
      switch(instrlist->instr->type){
        case E_INT:
          printf("\t%s: .space 4\n",  VARNAME.name);
          break;
        default:
          break;
      }
      instrlist = instrlist->next;
    }


  return;
}

void addStack()
{
  printf("\taddi $sp, $sp, -4\n");
  printf("\tsw $t0, 0($sp)\n");
}

void removeStack(char *name)
{
  printf("\tlw %s, 0($sp)\n", name);
  printf("\taddi $sp, $sp, 4\n");
}

void printText(InstrList* instrlist)
{
    printf(".text\n");

    while(instrlist->next != NULL)
    {
      switch(instrlist->instr->type){
        case E_LDC:
          printf("\tli $t0, %d\n",  VARNAME.value);
          addStack();
          break;
        case E_LDA:
          printf("\tla $t0, %s\n", VARNAME.name);
          addStack();
          break;
        case E_LOD:
          printf("\tlw $t0, %s\n", VARNAME.name);
          addStack();
          break;
        case E_ADI:
          removeStack("$t1");
          removeStack("$t0");
          printf("\tadd $t0, $t0, $t1\n");
          addStack();
          break;
        case E_SBI:
          removeStack("$t1");
          removeStack("$t0");
          printf("\tsub $t0, $t0, $t1\n");
          addStack();
          break;
        case E_DVI:
          removeStack("$t1");
          removeStack("$t0");
          printf("\tdiv $t0, $t1\n");
          printf("\tmove $t0, $LO\n");
          addStack();
          break;
        case E_MPI:
          removeStack("$t1");
          removeStack("$t0");
          printf("\tmult $t0, $t1\n");
          printf("\tmflo $t0\n");
          addStack();
          break;
        case E_STO:
          removeStack("$t1");
          removeStack("$t0");
          printf("\tsw $t1, 0($t0)\n");
          break;
        case E_LAB:
          printf("%s:\n", VARNAME.name);
          break;
        case E_INT:
          break;
        default:
          break;
      }
      instrlist = instrlist->next;
    }

  return;
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

    //InstrList* instr = compilePrintf(root5->varList);
    //printInstrPrintfList(instr);
    InstrList* instr = compileCmdList(root);
    printInstrCmdList(instr);

    printData(instr);
    printText(instr);
  }
  return 0;


}
