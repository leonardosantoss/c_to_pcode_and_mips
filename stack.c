#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

Instr* stack_instr_ldc(int value){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LDC;
    node->attr.value = value;
    return node;
}
Instr* stack_instr_adi(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_ADI;
    return node;
}
Instr* stack_instr_mpi(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_MPI;
    return node;
}
Instr* stack_instr_ges(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_GES;
    return node;
}
Instr* stack_instr_geq(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_GEQ;
    return node;
}
Instr* stack_instr_equ(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_EQU;
    return node;
}
Instr* stack_instr_les(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LES;
    return node;
}
Instr* stack_instr_leq(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LEQ;
    return node;
}
Instr* stack_instr_sbi(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_SBI;
    return node;
}

InstrList* stack_instrlist(Instr* instr, InstrList* next){
    InstrList* list = (InstrList*) malloc(sizeof(InstrList));
    list->instr = instr;
    list->next = next;
    return list;
}

Instr* stack_instr_rdi(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_RDI;
    return node;
}

Instr* stack_instr_wri(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_WRI;
    return node;
}
Instr* stack_instr_sto(){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_STO;
    return node;
}
Instr* stack_instr_lda(char* name){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LDA;
    node->attr.name = name;
    return node;
}
Instr* stack_instr_lod(char* name){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LOD;
    node->attr.name = name;
    return node;
}

Instr* stack_instr_var(char* name){
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_INT;
    node->attr.name = name;
    return node;
}

Instr* stack_instr_fjp(int label){
    char integer_string[32];
    char s1[32];
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_FJP;
    sprintf(integer_string, "%d", label);
    strcpy(s1, "L");
    strcat(s1, integer_string);
    node->attr.name = (char*) malloc(sizeof(char));
    strcpy(node->attr.name,s1);

    return node;
}

Instr* stack_instr_ujp(int label){
    char integer_string[32];
    char s1[32];
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_UJP;
    sprintf(integer_string, "%d", label);
    strcpy(s1, "L");
    strcat(s1, integer_string);
    node->attr.name = (char*) malloc(sizeof(char));
    strcpy(node->attr.name,s1);

    return node;
}

Instr* stack_instr_label(int label){
    char integer_string[32];
    char s1[32];
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LAB;
    sprintf(integer_string, "%d", label);
    strcpy(s1, "L");
    strcat(s1, integer_string);
    //printf("%s\n",s1);
    node->attr.name = (char*) malloc(sizeof(char));
    strcpy(node->attr.name,s1);
    return node;
}


int isEmpty(InstrList *root){
    return !root;
}

Instr* stack_instrlist_head(InstrList* list){
    if(isEmpty(list))
        return NULL;
    return list->instr;
}

InstrList* stack_instrlist_tail(InstrList* list){
    if(isEmpty(list))
        return NULL;
    return list->next;
}

void stack_instrlist_append(InstrList* instr1, InstrList* instr2){
    InstrList* tmp = (InstrList*) malloc (sizeof(InstrList));
    tmp = instr1;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = instr2;
}
