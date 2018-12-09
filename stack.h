
#ifndef __stack_h__
#define __stack_h__

struct _Instr{
    enum{
        E_LDC,
        E_LOD,
        E_LDA,

        E_STO,
        E_INT,

        E_RDI,
        E_WRI,

        E_ADI,
        E_MPI,
        E_SBI,
        E_MOD,
        E_DVI,

        E_UJP,
        E_FJP,

        E_EQU,
        E_GEQ,
        E_GES,
        E_LEQ,
        E_LES,
        E_NEQ,

        E_LAB
    }type;
    union{
        int value;
        char* name;
    }attr;

};

struct _InstrList{
    struct _Instr* instr;
    struct _InstrList* next;
};

typedef struct _Instr Instr;
typedef struct _InstrList InstrList;

int isEmpty(InstrList* list);

Instr* stack_instr_ldc(int value);
Instr* stack_instr_lod(char* name);
Instr* stack_instr_lda(char* name);
Instr* stack_instr_label(int label);
Instr* stack_instr_var(char* name);
Instr* stack_instr_fjp(int label);
Instr* stack_instr_ujp(int label);
Instr* stack_instr_adi();
Instr* stack_instr_mpi();
Instr* stack_instr_sbi();
Instr* stack_instr_wri();
Instr* stack_instr_rdi();
Instr* stack_instr_sto();
Instr* stack_instr_geq();
Instr* stack_instr_ges();
Instr* stack_instr_equ();
Instr* stack_instr_leq();
Instr* stack_instr_les();
Instr* stack_instr_neq();


InstrList* stack_instrlist(Instr* instr, InstrList* next);

Instr* stack_instrlist_head(InstrList* list);
InstrList* stack_instrlist_tail(InstrList* list);
void stack_instrlist_append(InstrList* list1, InstrList* list2);

#endif
