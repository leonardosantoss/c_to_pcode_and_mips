
#ifndef __stack_h__
#define __stack_h__

struct _Instr{
    enum{
        E_LDC,
        E_ADI,
        E_MPI,
        E_SBI
    }type;
    union{
        int value;
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
Instr* stack_instr_adi();
Instr* stack_instr_mpi();
Instr* stack_instr_sbi();

InstrList* stack_instrlist(Instr* instr, InstrList* next);

Instr* stack_instrlist_head(InstrList* list);
InstrList* stack_instrlist_tail(InstrList* list);
InstrList* stack_instrlist_append(InstrList* list1, InstrList* list2);



#endif